#include <WiFi.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include <DNSServer.h>

#define SSID_AP "HortaIOT"
#define SENHA_AP "senhaboa"

void configModeCallback (WiFiManager *myWiFiManager) {  
  Serial.println("Entrou no modo de configuração");
  Serial.println(WiFi.softAPIP()); //imprime o IP do AP
  Serial.println(myWiFiManager->getConfigPortalSSID()); //imprime o SSID criado da rede
}

void saveConfigCallback () {
  Serial.println("Configuração salva");
  Serial.println(WiFi.softAPIP()); //imprime o IP do AP
}

void setup() {
  Serial.begin(9600);
  WiFiManager wifiManager;
  //wifiManager.resetSettings();
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  //wifiManager.autoConnect(SSID_AP, SENHA_AP);
}

void loop() {
  WiFiManager wifiManager;
  //if(!wifiManager.startConfigPortal(SSID_AP, SENHA_AP) ){
  //  Serial.println("Falha ao conectar");
  //  delay(2000);
  //  ESP.restart();
  //  delay(1000);
  //}
}
