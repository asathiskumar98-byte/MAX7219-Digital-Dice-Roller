#include <SPI.h>

#define CS_PIN 10
#define BUTTON_PIN 2

// Dice face patterns (1–6) for 8×8 LED Matrix
// Each byte = one row (left→right bits = LED on/off)
const byte dicePatterns[6][8] = {
  {0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00}, // ⚀ 1 (bold center)
  {0xC0,0xC0,0x00,0x00,0x00,0x00,0x03,0x03}, // ⚁ 2 (top-left & bottom-right)
  {0xC0,0xC0,0x00,0x18,0x18,0x00,0x03,0x03}, // ⚂ 3 (diagonal)
  {0xC3,0xC3,0x00,0x00,0x00,0x00,0xC3,0xC3}, // ⚃ 4 (four corners)
  {0xC3,0xC3,0x00,0x18,0x18,0x00,0xC3,0xC3}, // ⚄ 5 (corners + center)
  {0xC3,0xC3,0x00,0xC3,0xC3,0x00,0xC3,0xC3}  // ⚅ 6 (three per side)
};

// Write one register of MAX7219
void write_to_MAX7219(byte addr, byte data) {
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(addr);
  SPI.transfer(data);
  digitalWrite(CS_PIN, HIGH);
}

// Initialize MAX7219
void MAX7219_init() {
  write_to_MAX7219(0x0C, 0x01); // Normal operation
  write_to_MAX7219(0x09, 0x00); // No decode mode
  write_to_MAX7219(0x0A, 0x08); // Medium brightness
  write_to_MAX7219(0x0B, 0x07); // Scan all 8 rows
  write_to_MAX7219(0x0F, 0x00); // Display test off
}

// Display a dice number (1–6)
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
  displayDice(0);  // Start with dice face 1
  randomSeed(analogRead(A0)); // Add randomness
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {  // Button pressed
    // Roll animation
    for (int i = 0; i < 10; i++) {
      byte roll = random(0, 6);
      displayDice(roll);
      delay(100 + i * 20);
    }
    // Final result
    byte finalRoll = random(0, 6);
    displayDice(finalRoll);
    delay(1000);
  }
}
