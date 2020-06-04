//bibioteca sensor temperatura e umidade
#include "DHT.h"
//bibioteca sensor distancia
#include <Ultrasonic.h>
 
#define DHTPIN 14 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11

//pinos sensor distancia
#define pino_trigger 13
#define pino_echo 12

//Cria objeto do sensor de distancia
Ultrasonic ultrasonic(pino_trigger, pino_echo);

//Cria objeto do sensor de temperatura e umidade
DHT dht(DHTPIN, DHTTYPE);

//variaveis a serem obtidas, leitura
float dist, temp;
long microsec;

float lerDistancia () {
  return ultrasonic.convert(microsec, Ultrasonic::CM);
}

float lerTemp () {
  return dht.readTemperature();
}

void printaDist () {
  Serial.print("Distancia em cm: ");
  Serial.println(dist);
}

void printTemp () {
  Serial.print("Temperatura: ");
  Serial.print(temp);
  Serial.println(" *C");
}

void setup() {
  Serial.begin(9600);
  Serial.println("Lendo dados do sensores...");
  dht.begin();
}

void loop() {
  microsec = ultrasonic.timing();
  dist = lerDistancia();
  printaDist();
  delay(500);
  
  temp = lerTemp();
  if (isnan(temp)) {
    Serial.println("Failed to read from DHT");
  } 
  else {
    printTemp();
  }
  //Serial.println("Esperando");
  delay(2500);
}
