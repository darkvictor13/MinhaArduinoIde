//bibliotecas a serem incluidas
#include <SSD1306Wire.h> 
#include "DHT.h"
#include <WiFi.h>

//pinos que serao usados
#define RELAY 12
#define DHT_PIN 5    
#define SCL 22
#define SDA 21
#define PIN_SOLO 15

//credenciais da rede 
#define SSID_AP "HortaIOT"
#define SSID_WIFI "SmartPTI"
#define SENHA_WIFI "SmartPT12017."

//Serve para identificar qual sensor de umidade e temperatura do ar estamos usando
#define DHT_TYPE DHT22 

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
