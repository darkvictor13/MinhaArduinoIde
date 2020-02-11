#include "DHT.h"

#define DHT_PIN 5    
#define DHT_TYPE DHT22

#define PIN_SOLO 15

float humidade, temperatura, umidade_do_solo;
String texto_temperatura = "";
String texto_umidade = "";
String texto_umidade_solo = "";

DHT dht(DHT_PIN, DHT_TYPE);

void iniciaSensorSolo(){
  pinMode(PIN_SOLO, INPUT);
}

void inicializaSensorHumETempAr(){
  Serial.println(F("Sensor Test"));
  dht.begin();
}

void medeUmidadeDoSolo() {
  umidade_do_solo = map (analogRead (PIN_SOLO), 4095, 1000, 0, 100);
  texto_umidade_solo = String(umidade_do_solo) + " % ";
  //delay(2000);
  Serial.print("Umidade do solo = ");
  Serial.println(umidade_do_solo);
}

void leituraSensorHumidadeEtemperaturaDoAr(){
  delay(2000);

  humidade = dht.readHumidity();
  temperatura = dht.readTemperature();  

  if (isnan(humidade) || isnan(temperatura)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    
  }
  texto_temperatura = String(temperatura) + " C ";
  texto_umidade = String(humidade) + " % ";
  Serial.print("Umidade do ar = ");
  Serial.println(humidade);
  Serial.print("Temperatura do ar = ");
  Serial.println(temperatura);
}

void lerTodosSensores() {
  medeUmidadeDoSolo();
  leituraSensorHumidadeEtemperaturaDoAr();  
}

void setup() {
  Serial.begin(9600);
  inicializaSensorHumETempAr();
  iniciaSensorSolo();
}

void loop() {
  lerTodosSensores();
}
