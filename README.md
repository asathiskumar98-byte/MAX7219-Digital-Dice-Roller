# 🎲 MAX7219 Digital Dice Roller

A simple **Arduino project** that simulates a rolling dice using an **8×8 LED Matrix (MAX7219)** and a **Push Button**.

---

## 🧠 Overview
When you press the button, the LED matrix displays a dice roll animation and stops on a random number between 1 and 6.  
Built using SPI communication with the MAX7219 driver.

---

## ⚙️ Hardware Components
- Arduino UNO  
- MAX7219 8×8 LED Matrix  
- Push Button  
- Breadboard + Jumper Wires  

---

## 🧩 Circuit Connections

| Component | Arduino Pin | Description |
|------------|-------------|--------------|
| MAX7219 CS | 10 | Chip Select |
| MAX7219 DIN | 11 | SPI MOSI |
| MAX7219 CLK | 13 | SPI Clock |
| Button | 2 | Input with Pullup |
| VCC | 5V | Power |
| GND | GND | Ground |

---

## 🧾 Code
```cpp
#include <SPI.h>

#define CS_PIN 10
#define BUTTON_PIN 2

const byte dicePatterns[6][8] = {
  {0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00},
  {0xC0,0xC0,0x00,0x00,0x00,0x00,0x03,0x03},
  {0xC0,0xC0,0x00,0x18,0x18,0x00,0x03,0x03},
  {0xC3,0xC3,0x00,0x00,0x00,0x00,0xC3,0xC3},
  {0xC3,0xC3,0x00,0x18,0x18,0x00,0xC3,0xC3},
  {0xC3,0xC3,0x00,0xC3,0xC3,0x00,0xC3,0xC3}
};

void write_to_MAX7219(byte addr, byte data) {
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(addr);
  SPI.transfer(data);
  digitalWrite(CS_PIN, HIGH);
}

void MAX7219_init() {
  write_to_MAX7219(0x0C, 0x01);
  write_to_MAX7219(0x09, 0x00);
  write_to_MAX7219(0x0A, 0x08);
  write_to_MAX7219(0x0B, 0x07);
  write_to_MAX7219(0x0F, 0x00);
}

void displayDice(byte num) {
  for (byte i = 0; i < 8; i++) {
    write_to_MAX7219(i + 1, dicePatterns[num][i]);
  }
}

void setup() {
  SPI.begin();
  pinMode(CS_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  MAX7219_init();
  displayDice(0);
  randomSeed(analogRead(A0));
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    for (int i = 0; i < 10; i++) {
      byte roll = random(0, 6);
      displayDice(roll);
      delay(100 + i * 20);
    }
    byte finalRoll = random(0, 6);
    displayDice(finalRoll);
    delay(1000);
  }
}
