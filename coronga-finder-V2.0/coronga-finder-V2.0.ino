// Bibioteca do sensorde temperatura e umidade
#include "DHT.h"
// Bibioteca do sensor de distancia
#include <Ultrasonic.h>

// Definindo pino do sensorde temperatura e umidade
#define DHTPIN 14 // Selecionando o pino
#define DHTTYPE DHT11 // Definindo o tipo de sensor

// Definindo pinos do sensor de distancia
#define PINOTRIGGER 13
#define PINOECHO 12

// Definindo pinos do relay
#define RELEY1 25
#define RELEY2 33

// Define constantes
#define MINDIST 0
#define MAXDIST 20
#define MINTEMP 20
#define MAXTEMP 23
#define TOTALREADTEMP 5

// Variaveis obtidas na leitura
float dist, temp, sumTemp, medTemp;
float readTemp[TOTALREADTEMP];
int loopReadTemp;

// Cria objeto do sensor de distancia
Ultrasonic ultrasonic(PINOTRIGGER, PINOECHO);

// Cria objeto do sensor de temperatura e umidade
DHT dht(DHTPIN, DHTTYPE);

// Ler distância
float lerDistancia () {
  long microsec = ultrasonic.timing();
  return ultrasonic.convert(microsec, Ultrasonic::CM);
}

// Ler temperatura
float lerTemp () {
  return dht.readTemperature();
}

// Print distância lida
void printDist () {
  Serial.print("Distancia em cm: ");
  Serial.println(dist);
}

// Print temperatura lida
void printTemp () {
  Serial.print("Temperatura: ");
  Serial.print(temp);
  Serial.println(" *C");
}

void onGreenLed () {
  digitalWrite(RELEY1, HIGH);
}

void onRedLed () {
  digitalWrite(RELEY2, HIGH);
}

void offGreenLed () {
  digitalWrite(RELEY1, LOW);
}

void offRedLed () {
  digitalWrite(RELEY2, LOW);
}

void setup() {

  // Definindo os modos dos pinos
  pinMode(RELEY1, OUTPUT);
  pinMode(RELEY2, OUTPUT);

  Serial.begin(9600);
  Serial.print("Iniciando dispositivos");
  for(int i = 0; i < 3; i++){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  Serial.print("Iniciando leituras");
  for(int i = 0; i < 3; i++){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  dht.begin();
  
}

void loop() {
  
  dist = lerDistancia();
  while ( !( MinDist < dist && dist < MaxDist ) ){
    dist = lerDistancia();
    Serial.println("Se aproxime ao sensor.");
  }

  delay(500);

  loopReadTemp = 0;
  while ( loopReadTemp < 5 ) {

    sumTemp = 0;

    Serial.println("-------------------------------");
    
    for(int i = 0; i < TOTALREADTEMP; i++){
      readTemp[i] = lerTemp();
      sumTemp += readTemp[i];
      Serial.print("Temperatura n");
      Serial.print(i+1);
      Serial.print(": ");
      Serial.println(readTemp[i]);
    }

    Serial.println("-------------------------------");

    medTemp = sumTemp / TOTALREADTEMP;

    if ( MINTEMP < medTemp && medTemp < MAXTEMP ){
      onGreenLed();
      Serial.println("Sua temperatura está correta. Pode continuar");
      delay(2000);
      break;
    }

    if ( MAXTEMP < medTemp ){
      onRedLed();
      Serial.println("Sua temperatura não está bem.");
      delay(2000);
      break;
    }

    loopReadTemp++;
  }

  offRedLed();
  offGreenLed();
  delay(3000);
}
