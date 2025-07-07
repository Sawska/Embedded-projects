# 🐍 LED Snake Game

A classic snake game implemented on an **ESP32** with an **8×8 LED matrix**, controlled using a **joystick module**. This project demonstrates real-time input handling, dynamic LED visualization, and game logic for collision detection and snake movement.

---

## 🧰 Hardware Overview

* **Board:** ESP32
* **Display:** MAX7219-based 8×8 LED Matrix
* **Input:** Joystick Module (VRX, VRY, SW)
* **Optional:** Button (for reset or pause)

---

## 🎮 Features

* Real-time joystick control
* Classic snake movement on an 8×8 grid
* Food spawning and growing tail
* Wall and self-collision detection
* Dynamic game speed increase as the snake grows
* Game over animation and automatic restart

---

## 🧠 Skills Demonstrated

* ✅ Game logic implementation
* ✅ Real-time analog input processing
* ✅ LED matrix visualization using `MD_MAX72XX`
* ✅ Collision detection and timing logic

---

## 🔌 Circuit Connections

| Module       | ESP32 Pin |
| ------------ | --------- |
| MAX7219 DIN  | GPIO 23   |
| MAX7219 CLK  | GPIO 18   |
| MAX7219 CS   | GPIO 5    |
| Joystick VRX | GPIO 34   |
| Joystick VRY | GPIO 35   |
| Joystick SW  | GPIO 32   |

> **Note:** The joystick switch (SW) is not used in the current version, but it's available for extensions (e.g., pause/reset).

---

## 🧾 Code

> **Libraries Required:**
>
> * [`MD_MAX72XX`](https://github.com/MajicDesigns/MD_MAX72XX)
> * `SPI` (built-in)

<details>
<summary>Click to view full Arduino code</summary>

```cpp
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 1

#define CS_PIN   5
#define CLK_PIN  18
#define DIN_PIN  23

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DIN_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// Joystick pins
const int VRX_PIN = 34;
const int VRY_PIN = 35;
const int SW_PIN  = 32;

const int WIDTH = 8;
const int HEIGHT = 8;

int snakeX[64], snakeY[64];
int snakeLength = 3;
int direction = 0; // 0-up, 1-right, 2-down, 3-left

int foodX = 5, foodY = 5;
unsigned long lastMoveTime = 0;
unsigned long moveInterval = 500;
bool gameOver = false;

void setup() {
  Serial.begin(115200);
  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY, 5);
  mx.clear();
  pinMode(SW_PIN, INPUT_PULLUP);

  snakeX[0] = 3; snakeY[0] = 4;
  snakeX[1] = 3; snakeY[1] = 5;
  snakeX[2] = 3; snakeY[2] = 6;

  randomSeed(analogRead(33));
  spawnFood();
}

void loop() {
  if (!gameOver) {
    handleInput();
    if (millis() - lastMoveTime > moveInterval) {
      moveSnake();
      lastMoveTime = millis();
    }
    draw();
  } else {
    showGameOver();
  }
}

void handleInput() {
  int x = analogRead(VRX_PIN);
  int y = analogRead(VRY_PIN);

  if (x < 1000 && direction != 3) direction = 1;
  if (x > 3000 && direction != 1) direction = 3;
  if (y < 1000 && direction != 2) direction = 0;
  if (y > 3000 && direction != 0) direction = 2;
}

void moveSnake() {
  int newX = snakeX[0];
  int newY = snakeY[0];

  switch (direction) {
    case 0: newY--; break;
    case 1: newX++; break;
    case 2: newY++; break;
    case 3: newX--; break;
  }

  if (newX < 0 || newX >= WIDTH || newY < 0 || newY >= HEIGHT || isOccupied(newX, newY)) {
    gameOver = true;
    return;
  }

  for (int i = snakeLength; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  snakeX[0] = newX;
  snakeY[0] = newY;

  if (newX == foodX && newY == foodY) {
    snakeLength++;
    spawnFood();
    if (moveInterval > 100) moveInterval -= 20;
  }
}

void spawnFood() {
  while (true) {
    int x = random(0, WIDTH);
    int y = random(0, HEIGHT);
    if (!isOccupied(x, y)) {
      foodX = x;
      foodY = y;
      return;
    }
  }
}

bool isOccupied(int x, int y) {
  for (int i = 0; i < snakeLength; i++) {
    if (snakeX[i] == x && snakeY[i] == y)
      return true;
  }
  return false;
}

void draw() {
  mx.clear();
  for (int i = 0; i < snakeLength; i++) {
    mx.setPoint(snakeY[i], snakeX[i], true);
  }
  mx.setPoint(foodY, foodX, true);
}

void showGameOver() {
  mx.clear();
  for (int i = 0; i < 8; i++) {
    mx.setRow(i, 0b10101010);
    delay(100);
  }
  delay(1000);
  mx.clear();
  delay(2000);
  resetGame();
}

void resetGame() {
  gameOver = false;
  snakeLength = 3;
  direction = 0;
  moveInterval = 500;
  snakeX[0] = 3; snakeY[0] = 4;
  snakeX[1] = 3; snakeY[1] = 5;
  snakeX[2] = 3; snakeY[2] = 6;
  spawnFood();
}
```

</details>

---

## Hardware 
![image](https://github.com/user-attachments/assets/e97b92f3-c2da-43d4-b143-265eed5633aa)


## 🧭 Circuit Diagram

![Circuit Diagram](https://github.com/user-attachments/assets/1d257cab-0710-4cf1-a127-42b07e872a3b)

Simulate the project live:
👉 [**Wokwi Simulation Link**](https://wokwi.com/projects/435090792315382785)

---

## 🧩 Future Improvements

* Display score via Serial or OLED
* Use SW pin for pause or reset
* Multi-speed levels
* Game level/obstacles
* Sound buzzer on game over

