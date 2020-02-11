#include <SSD1306Wire.h>

#define scl 22//5
#define sda 21//18

SSD1306Wire display(0x3c, SDA, SCL);
void setup() {
  display.init();

  display.setColor(WHITE); 
  display.setFont(ArialMT_Plain_16);
  display.drawRect(0, 0, 128, 64);
  display.drawString(2,2,"Bem vindo\nInicializando");
  display.display();
  delay (1000);
  display.clear();
  
  display.drawRect(0, 0, 128, 64);
  display.drawString(2, 15, "  Iniciando");
  display.display();
  display.setFont(ArialMT_Plain_24);
  delay (1000);
  display.drawRect(19, 31, 101, 33);
  for (int i = 20; i <= 100; i++) {
    display.drawString(i, 32, ".");
    display.display();
    delay (25);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
