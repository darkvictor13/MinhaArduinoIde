#include <Wire.h>
#include "SSD1306.h"

#define SCL 22
#define SDA 21
#define PIN_SOLO 15

float umidade;

SSD1306 display(0x3c, SDA, SCL);

void setup() {
  display.init();
  pinMode(PIN_SOLO, INPUT);
}

void loop() {
  umidade = analogRead(PIN_SOLO);
  display.drawString(0, 0, String(umidade));
  display.display();
  delay (2000);
  display.clear();
}
