#include <SSD1306Wire.h> 
#include "DHT.h"

#define RELE 12
#define DHTPIN 5    
#define DHTTYPE DHT22 
#define scl 22
#define sda 21
#define PINSOLO 15

#define HUM_SOLO_MIN 30
#define HUM_AR_MIN 60
#define TEMP_AR_MAX 30

SSD1306Wire  display(0x3c, sda, scl);
DHT dht(DHTPIN, DHTTYPE);

float humidade = 0, temperatura = 0, umidadeSolo;
String textoTemperatura ="";
String textoHumidade ="";
String textoUmidadeSolo;


void ligarBomba(){
  digitalWrite(RELE,HIGH);
}

void desligarBomba(){
  digitalWrite(RELE,LOW);
}

void medeUmidadeDoSolo() {
  umidadeSolo = map (analogRead (PINSOLO),4095,1000,0,100);
  textoUmidadeSolo = String(umidadeSolo) + " % ";
  delay(2000);
}

void iniciaSensorSolo(){
  pinMode(PINSOLO, INPUT);
}

void inicializaSensorHumETempAr(){
  Serial.println(F("Sensor Test"));
  dht.begin();
}

void leituraSensorHumidadeEtemperaturaDoAr(){
  delay(2000);

  humidade = dht.readHumidity();
  temperatura = dht.readTemperature();  

  if (isnan(humidade) || isnan(temperatura)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    
  }
  textoTemperatura = String(temperatura) + " C ";
  textoHumidade = String(humidade) + " % ";
}

void imprimeDadosNoDisplay(){
  display.clear();
  Serial.println("Imprimindo no display");
  Serial.println(textoTemperatura);
  Serial.println(textoHumidade);
  Serial.println(textoUmidadeSolo);
  
  display.setFont(ArialMT_Plain_16);
  display.drawRect(0, 0, 128, 64);
  //display.setTextAlignment(TEXT_ALIGN_CENTER);
  //display.drawString(50, 0, textoTemperatura + textoHumidade + textoUmidadeSolo);
  display.drawString(10, 2, "Temp : " + textoTemperatura + "\n");
  display.drawString(10, 20, "Umid : " + textoHumidade + "\n");
  display.drawString(10, 40, "SoLo : " + textoUmidadeSolo + "\n");
  display.display();
}

void lerTodosSensores(){
  medeUmidadeDoSolo();
  leituraSensorHumidadeEtemperaturaDoAr();  
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

void tomaAcoes () {
  if (umidadeSolo < HUM_SOLO_MIN) {
    ligarBomba();
  }else {
    desligarBomba();
  }

  if (temperatura > TEMP_AR_MAX) mensagemTempAlta();
  if (humidade < HUM_AR_MIN) mensagemHumBaixa();
}

void setup() {
  
  //Inicia Monitor Serial
  Serial.begin(9600);

  //Incia sensor de temperatura e humidade do ar
  inicializaSensorHumETempAr();

  //Inicia Display oled
  display.init();

  //Inicia Sensor de umidade do solo
  iniciaSensorSolo();

  pinMode(RELE, OUTPUT);
  
}

void loop() {  

  lerTodosSensores();
  tomaAcoes();   
  imprimeDadosNoDisplay();
  
}
