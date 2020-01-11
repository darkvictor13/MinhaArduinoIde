#include <WiFi.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include <DNSServer.h>

#define SSID_AP "HortaIOT"
#define SENHA_AP "senhaboa"

String ssid_conectada;

void configModeCallback (WiFiManager *myWiFiManager) {  
  Serial.println("Entrou no modo de configuração");
  Serial.println(WiFi.softAPIP()); //imprime o IP do AP
  Serial.println(myWiFiManager->getConfigPortalSSID()); //imprime o SSID criado da rede
}

void saveConfigCallback () {
//  Serial.println("Should save config");
  Serial.println("Configuração salva");
  Serial.println(WiFi.softAPIP()); //imprime o IP do AP
}

void setup() {
  Serial.begin(9600);
  WiFiManager wifiManager;
  //wifiManager.setSTAStaticIPConfig(IPAddress(172,25,239,150),IPAddress(192,168,4,1), IPAddress(255,255,255,0));
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.autoConnect(SSID_AP, SENHA_AP);
}

void loop() {
  WiFiManager wifiManager;
  if(!wifiManager.startConfigPortal(SSID_AP, SENHA_AP) ){
    Serial.println("Falha ao conectar");
    delay(2000);
    ESP.restart();
    delay(1000);
  }
  Serial.print("Voce esta conectado a : ");
  ssid_conectada = wifiManager.getConfigPortalSSID();
  Serial.println(ssid_conectada);
}
