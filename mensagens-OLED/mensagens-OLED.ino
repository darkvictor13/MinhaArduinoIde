#include <SSD1306Wire.h> 

#define scl 18
#define sda 19

SSD1306Wire  display(0x3c, sda, scl);

void setup() {
  display.init();
}

void loop() {
  display.clear();
  display.setFont(ArialMT_Plain_16);
  //display.setColor(WHITE);
  display.drawRect(0, 0, 128, 22);
  display.drawString(4, 2, "Umid do ar baixa");

  display.setFont(ArialMT_Plain_16);
  display.drawString(10, 27, "  Mude a horta\n");
  display.drawString(35, 45,"de lugar\n");
  display.display();
  delay(1000);
}
