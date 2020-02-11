#include <Wire.h>
#include "SSD1306.h"
 
SSD1306 display(0x3c, 21, 22);
 
 
void setup() {                
  display.init();
  display.setFont(ArialMT_Plain_24);
  display.drawRect(0, 0, 128, 48);
  display.drawString(2,5," Bem vindo");
  //display.drawCircle(64, 52, 8);
  display.display();
  delay (2000);
  display.clear();
  
  display.drawRect(0, 0, 128, 64);
  display.drawString(2, 15, "  Iniciando");
  display.display();
  display.setFont(ArialMT_Plain_24);
  delay (1000);
  display.drawRect(21, 52, 83, 3);
  for (int i = 20; i <= 100; i++) {
    display.drawString(i, 32, ".");
    display.display();
    delay (35);
  }
}
 
 
void loop() 
{
 
}
