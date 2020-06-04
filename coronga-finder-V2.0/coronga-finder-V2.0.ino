// Bibioteca do sensorde temperatura e umidade
#include "DHT.h"
// Bibioteca do sensor de distancia
#include <Ultrasonic.h>

// Definindo pino do sensorde temperatura e umidade
#define DHTPIN 14 // Selecionando o pino
#define DHTTYPE DHT11 // Definindo o tipo de sensor

// Definindo pinos do sensor de distancia
#define PinoTrigger 13
#define PinoEcho 12

// Definindo pinos do relay
#define Reley1 25
#define Reley2 33

// Define constantes
#define MinDist 0
#define MaxDist 20
#define MinTemp 20
#define MaxTemp 23
#define TotalReadTemp 5

// Variaveis obtidas na leitura
float dist, temp, sumTemp, medTemp;
float readTemp[TotalReadTemp];
int loopReadTemp;

// Cria objeto do sensor de distancia
Ultrasonic ultrasonic(PinoTrigger, PinoEcho);

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
  digitalWrite(Reley1, HIGH);
}

void onRedLed () {
  digitalWrite(Reley2, HIGH);
}

void offGreenLed () {
  digitalWrite(Reley1, LOW);
}

void offRedLed () {
  digitalWrite(Reley2, LOW);
}

void setup() {

  // Definindo os modos dos pinos
  pinMode(Reley1, OUTPUT);
  pinMode(Reley2, OUTPUT);

  Serial.println();
  Serial.println();
  Serial.println();
  
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
    
    for(int i = 0; i < TotalReadTemp; i++){
      readTemp[i] = lerTemp();
      sumTemp += readTemp[i];
      Serial.print("Temperatura n");
      Serial.print(i+1);
      Serial.print(": ");
      Serial.println(readTemp[i]);
    }

    Serial.println("-------------------------------");

    medTemp = sumTemp / TotalReadTemp;

    if ( MinTemp < medTemp && medTemp < MaxTemp ){
      onGreenLed();
      Serial.println("Sua temperatura está correta. Pode continuar");
      delay(2000);
      break;
    }

    if ( MaxTemp < medTemp ){
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
