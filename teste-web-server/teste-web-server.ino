#include <WiFi.h>

#define SSID_WIFI "SmartPTI"
#define SENHA_WIFI "SmartPT12017."

WiFiClient cliente;
WiFiServer server(80);

String header;

void conectaNoWifi() {
 WiFi.begin(SSID_WIFI, SENHA_WIFI);

 while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.println("Connecting to WiFi..");
 }
 Serial.println("Connected to the WiFi network"); 
 server.begin(); 
}

void setup() {
  Serial.begin(115200);
  conectaNoWifi();
}

void loop() {
  cliente = server.available();
  if (cliente == true) {
    Serial.println("Cheguei");
      String req = cliente.readStringUntil('\r');//Faz a leitura do Cliente.
      Serial.println(req);//Printa o pedido no Serial monitor.
      cliente.println("Ola mundo");
  }
}
