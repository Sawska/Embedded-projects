#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_pm.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "dht.h" 

#define DHT_GPIO GPIO_NUM_4

static const char* TAG = "ClockSwitch";

void app_main(void) {
    
    esp_pm_config_esp32_t pm_config = {
        .max_freq_mhz = 160,
        .min_freq_mhz = 10,
        .light_sleep_enable = true
    };

    ESP_ERROR_CHECK(esp_pm_configure(&pm_config));
    ESP_LOGI(TAG, "Power management configured");

    while (1) {
    
        esp_pm_lock_handle_t lock;
        esp_pm_lock_create(ESP_PM_CPU_FREQ_MAX, 0, "sensor_read", &lock);
        esp_pm_lock_acquire(lock);

        ESP_LOGI(TAG, "Reading sensor at high frequency");
        float temperature = 0, humidity = 0;
        if (dht_read_float_data(DHT_TYPE_DHT22, DHT_GPIO, &humidity, &temperature) == ESP_OK) {
            ESP_LOGI(TAG, "Temp: %.1f C, Humidity: %.1f %%", temperature, humidity);
        } else {
            ESP_LOGE(TAG, "Failed to read DHT22 sensor");
        }

        esp_pm_lock_release(lock);
        esp_pm_lock_delete(lock);

        
        ESP_LOGI(TAG, "Entering idle (low-frequency) mode...");
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
