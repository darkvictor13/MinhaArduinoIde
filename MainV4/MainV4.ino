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
#define SENHA_AP "senhaboa"

//Serve para identificar qual sensor de umidade e temperatura do ar estamos usando
#define DHT_TYPE DHT22 

//valores de referencia para serem usados em comparacao
#define HUM_SOLO_MIN 30
#define HUM_AR_MIN 60
#define TEMP_AR_MAX 30

//inicializacoes antes de setup
WiFiServer server(80);
SSD1306Wire display(0x3c, SDA, SCL);
DHT dht(DHT_PIN, DHT_TYPE);

//variaveis para guardar os dados lidos pelo Esp
float humidade, temperatura, umidade_do_solo;
String texto_temperatura = "";
String texto_umidade = "";
String texto_umidade_solo = "";

//funcoes de inicializacao

void ligarBomba() {
  digitalWrite(RELAY, HIGH);
}

void desligarBomba() {
  digitalWrite(RELAY, LOW);
}

void iniciaSensorSolo() {
  pinMode(PIN_SOLO, INPUT);
}

void inicializaSensorHumETempAr() {
  Serial.println(F("Sensor Test"));
  dht.begin();
}

//funcoes de leitura

void medeUmidadeDoSolo() {
  umidade_do_solo = map (analogRead (PIN_SOLO), 4095, 1000, 0, 100);
  texto_umidade_solo = String(umidade_do_solo) + " % ";
  delay(2000);
}

void leituraSensorHumidadeEtemperaturaDoAr() {
  delay(2000);

  humidade = dht.readHumidity();
  temperatura = dht.readTemperature();  

  if (isnan(humidade) || isnan(temperatura)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    
  }
  texto_temperatura = String(temperatura) + " C ";
  texto_umidade = String(humidade) + " % ";
}

void lerTodosSensores() {
  medeUmidadeDoSolo();
  leituraSensorHumidadeEtemperaturaDoAr();  
}

//funcoes sobre o OLED

void imprimeDadosNoDisplay() {
  display.clear();
  Serial.println("Imprimindo no display");
  Serial.println(texto_temperatura);
  Serial.println(texto_umidade);
  Serial.println(texto_umidade_solo);
  
  display.setFont(ArialMT_Plain_16);
  display.drawRect(0, 0, 128, 64);
  //display.setTextAlignment(TEXT_ALIGN_CENTER);
  //display.drawString(50, 0, texto_temperatura + texto_umidade + textoUmidadeSolo);
  display.drawString(10, 2, "Temp : " + texto_temperatura + "\n");
  display.drawString(10, 20, "Umid : " + texto_umidade + "\n");
  display.drawString(10, 40, "SoLo : " + texto_umidade_solo + "\n");
  display.display();
}

void mensagemTempAlta() {
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawRect(0, 0, 128, 22);
  display.drawString(2, 12, "Temperatura alta");//16

  display.setFont(ArialMT_Plain_16);
  display.drawString(10, 27, "  Mude a horta\n");
  display.drawString(35, 45,"de lugar\n");
  delay(1000);
}

void mensagemHumBaixa() {
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawRect(0, 0, 128, 22);
  display.drawString(4, 2, "Umid do ar baixa");//16

  display.setFont(ArialMT_Plain_16);
  display.drawString(10, 27, "  Mude a horta\n");
  display.drawString(35, 45,"de lugar\n");
  display.display();
  delay(1000);
}

//acoes que devem ser tomadas quando um dado eh lido

void tomaAcoes() {
  if (umidade_do_solo < HUM_SOLO_MIN) {
    ligarBomba();
  }else {
    desligarBomba();
  }

  if (temperatura > TEMP_AR_MAX) mensagemTempAlta();
  if (humidade < HUM_AR_MIN) mensagemHumBaixa();
}

void configAp() {
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(SSID_AP, SENHA_AP);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();
}

void dadosHtml() {
  
}

void setup() {
  //Inicia Monitor Serial
  Serial.begin(115200);//9600

  //Incia sensor de temperatura e humidade do ar
  inicializaSensorHumETempAr();

  //configurando AP
  configAp();

  //Inicia Display oled
  display.init();

  //Inicia Sensor de umidade do solo
  iniciaSensorSolo();

  //Inicia o relay
  pinMode(RELAY, OUTPUT);
}

void loop() {
  WiFiClient client = server.available();
  //ClientRequest = (ReadIncomingRequest());
  //if (client) {
    Serial.println("Cheguei");
    while (client.connected()) {
      Serial.println("Cheguei 2");
      if (client.available()) {
        Serial.println("Cheguei 3");
        lerTodosSensores();
        tomaAcoes();   
        imprimeDadosNoDisplay();
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println("");
        client.println("<!DOCTYPE HTML>");
        client.print("Temp do ar : ");
        client.print(texto_temperatura);
        //client.print("<br />");
        client.print("Humid do ar : ");
        client.print(texto_umidade);
        //client.print("<br />");
        client.print("Humid do solo : ");
        client.print(texto_umidade_solo);
        //client.print("<br />");
        client.println("</html>");
      }
    //}
  }
  delay(2000);
  //Close the connection
  client.stop();
  Serial.println("Client disconnected.");
  Serial.println("");
}
