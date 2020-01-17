#include <SSD1306Wire.h>

#define scl 5//22
#define sda 18//21

SSD1306Wire  display(0x3c, sda, scl);

void setup() {
  display.init();
  display.drawString(0, 0, "Hello World");
  display.display();

}

void loop() {
  // put your main code here, to run repeatedly:

}
