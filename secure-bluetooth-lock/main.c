#include <string.h>
#include <stdio.h>
#include <inttypes.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "driver/gpio.h"

#include "mbedtls/gcm.h"

#include "nimble/ble.h"
#include "host/ble_hs.h"
#include "host/ble_gatt.h"
#include "host/ble_uuid.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"

#define TAG "SEC_LOCK"

#define LOCK_GPIO              GPIO_NUM_4
#define UNLOCK_PULSE_MS        2000

static const uint8_t AES_KEY_128[16] = {
    0x23,0x42,0xA1,0x5C,0x99,0x77,0xE2,0x10,0x5B,0x6C,0xDE,0xAB,0xF1,0x32,0x0E,0x7D
};

static const char *EXPECTED_TOKEN = "UNLOCK-123456";

#define NONCE_LEN 12
#define TAG_LEN   16
#define NONCE_CACHE 16

static const ble_uuid128_t SERVICE_UUID =
    BLE_UUID128_INIT(0x32,0x8c,0x54,0x89,0x21,0x4a,0x43,0x2b,0x9f,0x25,0x3a,0x91,0xf0,0xaa,0x11,0x01);
static const ble_uuid128_t RX_CHAR_UUID =
    BLE_UUID128_INIT(0x32,0x8c,0x54,0x89,0x21,0x4a,0x43,0x2b,0x9f,0x25,0x3a,0x91,0xf0,0xaa,0x11,0x02);
static const ble_uuid128_t TX_CHAR_UUID =
    BLE_UUID128_INIT(0x32,0x8c,0x54,0x89,0x21,0x4a,0x43,0x2b,0x9f,0x25,0x3a,0x91,0xf0,0xaa,0x11,0x03);

static uint16_t tx_handle = 0;
static uint16_t rx_handle = 0;
static uint16_t conn_handle = 0xffff;

typedef enum {
    LOCK_CMD_UNLOCK = 1
} lock_cmd_t;

static QueueHandle_t lock_queue;

static uint8_t nonce_cache[NONCE_CACHE][NONCE_LEN];
static size_t nonce_idx = 0;
static bool nonce_cache_inited = false;

static void nonce_cache_init(void) {
    memset(nonce_cache, 0, sizeof(nonce_cache));
    nonce_idx = 0;
    nonce_cache_inited = true;
}

static bool nonce_seen(const uint8_t *nonce) {
    if (!nonce_cache_inited) return false;
    for (size_t i = 0; i < NONCE_CACHE; ++i) {
        if (memcmp(nonce_cache[i], nonce, NONCE_LEN) == 0) return true;
    }
    return false;
}

static void nonce_store(const uint8_t *nonce) {
    memcpy(nonce_cache[nonce_idx], nonce, NONCE_LEN);
    nonce_idx = (nonce_idx + 1) % NONCE_CACHE;
}

static void lock_task(void *arg) {
    gpio_config_t io = {
        .pin_bit_mask = 1ULL << LOCK_GPIO,
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = 0,
        .pull_up_en = 0,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io);
    gpio_set_level(LOCK_GPIO, 0);

    lock_cmd_t cmd;
    while (true) {
        if (xQueueReceive(lock_queue, &cmd, portMAX_DELAY)) {
            if (cmd == LOCK_CMD_UNLOCK) {
                ESP_LOGI(TAG, "Unlock pulse start");
                gpio_set_level(LOCK_GPIO, 1);
                vTaskDelay(pdMS_TO_TICKS(UNLOCK_PULSE_MS));
                gpio_set_level(LOCK_GPIO, 0);
                ESP_LOGI(TAG, "Unlock pulse end");
            }
        }
    }
}

static int decrypt_and_check(const uint8_t *nonce,
                             const uint8_t *ciphertext, size_t cipher_len,
                             const uint8_t *tag,
                             uint8_t *out_plain ) {
    mbedtls_gcm_context gcm;
    mbedtls_gcm_init(&gcm);
    int rc = mbedtls_gcm_setkey(&gcm, MBEDTLS_CIPHER_ID_AES, AES_KEY_128, 128);
    if (rc != 0) { mbedtls_gcm_free(&gcm); return rc; }

    rc = mbedtls_gcm_auth_decrypt(&gcm,
                                  cipher_len,
                                  nonce, NONCE_LEN,
                                  NULL, 0,
                                  tag, TAG_LEN,
                                  ciphertext, out_plain);
    mbedtls_gcm_free(&gcm);
    return rc; 
}

static void notify_text(const char *msg) {
    if (conn_handle != 0xffff && tx_handle != 0) {
        struct os_mbuf *om = ble_hs_mbuf_from_flat(msg, strlen(msg));
        if (om) {
            ble_gattc_notify_custom(conn_handle, tx_handle, om);
        }
    }
}

static int on_rx_write(uint16_t conn_handle_cb, uint16_t attr_handle,
                       struct ble_gatt_access_ctxt *ctxt, void *arg) {
    (void)conn_handle_cb; (void)attr_handle; (void)arg;
    if (ctxt->op != BLE_GATT_ACCESS_OP_WRITE_CHR) return BLE_ATT_ERR_WRITE_NOT_PERMITTED;

    uint16_t len = OS_MBUF_PKTLEN(ctxt->om);
    if (len < NONCE_LEN + TAG_LEN + 1) {
        notify_text("ERR_FORMAT");
        return 0;
    }

    uint8_t *buf = malloc(len);
    if (!buf) {
        notify_text("ERR_MEM");
        return 0;
    }
    os_mbuf_copydata(ctxt->om, 0, len, buf);

    const uint8_t *nonce = buf;
    const uint8_t *tag   = buf + (len - TAG_LEN);
    const uint8_t *cipher = buf + NONCE_LEN;
    size_t cipher_len = len - NONCE_LEN - TAG_LEN;

    if (nonce_seen(nonce)) {
        notify_text("REPLAY");
        free(buf);
        return 0;
    }

    uint8_t *plain = malloc(cipher_len + 1);
    if (!plain) {
        notify_text("ERR_MEM");
        free(buf);
        return 0;
    }

    int rc = decrypt_and_check(nonce, cipher, cipher_len, tag, plain);
    if (rc != 0) {
        notify_text("BAD_TAG");
        free(plain);
        free(buf);
        return 0;
    }

    plain[cipher_len] = '\0';

    size_t exp_len = strlen(EXPECTED_TOKEN);
    if (cipher_len == exp_len && memcmp(plain, EXPECTED_TOKEN, exp_len) == 0) {
        nonce_store(nonce);
        notify_text("OK");
        lock_cmd_t cmd = LOCK_CMD_UNLOCK;
        xQueueSend(lock_queue, &cmd, 0);
    } else {
        notify_text("BAD_TOKEN");
    }

    free(plain);
    free(buf);
    return 0;
}

static const struct ble_gatt_chr_def gatt_chars[] = {
    {
        .uuid = &RX_CHAR_UUID.u,
        .access_cb = on_rx_write,
        .flags = BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_WRITE_NO_RSP,
        .val_handle = &rx_handle,
    },
    {
        .uuid = &TX_CHAR_UUID.u,
        .access_cb = NULL,
        .flags = BLE_GATT_CHR_F_NOTIFY,
        .val_handle = &tx_handle,
    },
    { 0 } 
};

static const struct ble_gatt_svc_def gatt_svcs[] = {
    {
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = &SERVICE_UUID.u,
        .characteristics = gatt_chars,
    },
    { 0 } 
};

static int gap_event(struct ble_gap_event *event, void *arg) {
    (void)arg;
    switch (event->type) {
    case BLE_GAP_EVENT_CONNECT:
        if (event->connect.status == 0) {
            conn_handle = event->connect.conn_handle;
            ESP_LOGI(TAG, "Connected, handle=%u", conn_handle);
        } else {
            ESP_LOGI(TAG, "Connect failed; retrying adv");
            ble_gap_adv_start(BLE_OWN_ADDR_PUBLIC, NULL, BLE_HS_FOREVER,
                              &(struct ble_gap_adv_params){ .conn_mode = BLE_GAP_CONN_MODE_UND,
                                                            .disc_mode = BLE_GAP_DISC_MODE_GEN },
                              gap_event, NULL);
        }
        return 0;

    case BLE_GAP_EVENT_DISCONNECT:
        ESP_LOGI(TAG, "Disconnected");
        conn_handle = 0xffff;
        ble_gap_adv_start(BLE_OWN_ADDR_PUBLIC, NULL, BLE_HS_FOREVER,
                          &(struct ble_gap_adv_params){ .conn_mode = BLE_GAP_CONN_MODE_UND,
                                                        .disc_mode = BLE_GAP_DISC_MODE_GEN },
                          gap_event, NULL);
        return 0;

    case BLE_GAP_EVENT_SUBSCRIBE:
        ESP_LOGI(TAG, "Subscribe: cur_notify=%d", event->subscribe.cur_notify);
        return 0;

    default:
        return 0;
    }
}

static void ble_on_sync(void) {
    ble_svc_gap_device_name_set("SecureLock-ESP32");

    uint8_t addr_val[6] = {0};
    ble_hs_id_infer_auto(0, &addr_val[0]);

    struct ble_gap_adv_params advp = {
        .conn_mode = BLE_GAP_CONN_MODE_UND,
        .disc_mode = BLE_GAP_DISC_MODE_GEN
    };
    ble_gap_adv_start(BLE_OWN_ADDR_PUBLIC, NULL, BLE_HS_FOREVER, &advp, gap_event, NULL);
    ESP_LOGI(TAG, "Advertising started");
}

static void on_reset(int reason) {
    ESP_LOGW(TAG, "BLE reset, reason=%d", reason);
}

void app_main(void) {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_LOGI(TAG, "Booting…");

    lock_queue = xQueueCreate(4, sizeof(lock_cmd_t));
    xTaskCreate(lock_task, "lock_task", 2048, NULL, 4, NULL);

    nonce_cache_init();

    ESP_ERROR_CHECK(ble_hs_init());
    ble_hs_cfg.reset_cb = on_reset;
    ble_hs_cfg.sync_cb = ble_on_sync;

    ble_svc_gap_init();
    ble_svc_gatt_init();
    ESP_ERROR_CHECK(ble_gatts_count_cfg(gatt_svcs));
    ESP_ERROR_CHECK(ble_gatts_add_svcs(gatt_svcs));

    nimble_port_freertos_init(NULL);
}
