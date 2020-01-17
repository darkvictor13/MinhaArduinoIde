#include <SSD1306Wire.h>//biblioteca do Oled
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>

#define SSID_AP "HortaIOT"
#define SENHA_AP "senhacriativa"

#define pin_relay 13//5
#define pin_solo 15//36
#define pin_botao_config 
#define scl 5//22
#define sda 18//21
#define umidade_solo_minima 20.1

float umidade_solo_porcentagem;
int erro;

//callback que indica que o ESP entrou no modo AP
void configModeCallback (WiFiManager *myWiFiManager) {  
  Serial.println("Entrou no modo de configuração");
  Serial.println(WiFi.softAPIP()); //imprime o IP do AP
  Serial.println(myWiFiManager->getConfigPortalSSID()); //imprime o SSID criado da rede
}

//callback que indica que salvamos uma nova rede para se conectar (modo estação)
void saveConfigCallback () {
  Serial.println("Configuração salva");
}

void medeUmidadeDoSoloSalvaEmPorcentagem () {
  umidade_solo_porcentagem = map (analogRead (pin_solo),4095,1000,0,100);
}

void verificaErros () {
  if (umidade_solo_porcentagem <= umidade_solo_minima) {//valor de humidade do solo que a planta acha ruim
    erro = 1;
  }else {
    erro = 0;
  }
}

void MostraParaUsuario() {//faz exatamente oq o nome fala =)
  if (erro) {
    display.clear();
    display.setFont(ArialMT_Plain_24);
    display.drawRect(0, 0, 128, 64);
    display.drawString(2,10,"  Irrigando\n");
    display.display();
    delay (100);
    display.setFont(ArialMT_Plain_16);
    display.drawString(8,37," umidade baixa\n");
    display.display();//faz todos os comandos aparecerem no oled
  }else {
    display.clear();
    display.setFont(ArialMT_Plain_24);
    display.drawRect(0, 0, 128, 64);
    display.drawString(0,5,"  Aqumatic  \n  Saudável\n");
    display.display();
  }
}

void realizaAcoes () {
  if (erro) {
    digitalWrite(pin_relay, HIGH);
  }else {
    digitalWrite(pin_relay, LOW);
  }
}

SSD1306Wire  display(0x3c, sda, scl);

void setup() {
  Serial.begin(9600);
  WiFiManager wifiManager;
  //wifiManager.resetSettings();
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.autoConnect(SSID_AP, SENHA_AP);

  pinMode(pin_solo,INPUT);
  pinMode(pin_relay, OUTPUT);
  display.init();//inicia o display do oled
  display.setFont(ArialMT_Plain_24);
}

void loop() {
  WiFiManager wifiManager;
  if (digitalRead(pin_botao_config) == HIGH){//entra no modo de configuracao do wifi
     if(!wifiManager.startConfigPortal(SSID_AP, SENHA_AP) ) {
       Serial.println("Falha ao conectar");
     }
  }

  
}
