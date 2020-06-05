// Bibioteca do sensor de tempCorperatura
#include <Wire.h>
#include <Adafruit_MLX90614.h>                                         
// Bibioteca do sensor de distancia                      
#include <Ultrasonic.h>                                  
///////////////////////////////////////////////////////////

// Definindo pinos do sensor de distancia /////////////////
#define PINOTRIGGER 13
#define PINOECHO 12
///////////////////////////////////////////////////////////

// Definindo pinos do relay ///////////////////////////////
#define RELEY1 25                                   
#define RELEY2 26                                    
///////////////////////////////////////////////////////////

// Define constantes //////////////////////////////////////
#define MINDIST 0                                       
#define MAXDIST 20                                      
#define MINTEMP 0                                    
#define MAXTEMP 10                                 
#define TOTALREADTEMP 20                                 
///////////////////////////////////////////////////////////

// Variaveis obtidas na leitura ///////////////////////////
float dist, tempCorp, tempAmb, sumTemp, medTemp;                     
float readTemp[TOTALREADTEMP];                          
int loopReadTemp;                              
///////////////////////////////////////////////////////////

// Cria objeto do sensor de distancia /////////////////////
Ultrasonic ultrasonic(PINOTRIGGER, PINOECHO);            
///////////////////////////////////////////////////////////

// Cria objeto do sensor de tempCorperatura e umidade /////////
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
///////////////////////////////////////////////////////////

// Ler distância //////////////////////////////////////////
float lerDistancia () {
  long microsec = ultrasonic.timing();
  return ultrasonic.convert(microsec, Ultrasonic::CM);
}
//////////////////////////////////////////////////////////

// Ler temperatura ///////////////////////////////////
float lerTemp () {
  return mlx.readObjectTempC();
}
//////////////////////////////////////////////////////////

// Print distância lida ///////////////////////////////
void printDist () {
  Serial.print("Distancia em cm: ");
  Serial.println(dist);
}
//////////////////////////////////////////////////////////

// Print tempCorperatura lida ///////////////////////////////
void printTemp () {
  Serial.print("Temperatura: ");
  Serial.print(tempCorp);
  Serial.println(" *C");
}
//////////////////////////////////////////////////////////

// Liga o led verde ///////////////////////////////////
void onGreenLed () {
  digitalWrite(RELEY1, HIGH);
}
//////////////////////////////////////////////////////////

// Liga o led vermelho ///////////////////////////////////
void onRedLed () {
  digitalWrite(RELEY2, HIGH);
}
//////////////////////////////////////////////////////////

// Desliga o led verde ///////////////////////////////////
void offGreenLed () {
  digitalWrite(RELEY1, LOW);
}
//////////////////////////////////////////////////////////

// Desliga o led vermelho ///////////////////////////////
void offRedLed () {
  digitalWrite(RELEY2, LOW);
}
//////////////////////////////////////////////////////////

// funcao setup //////////////////////////////////////////
void setup() {
  // Definindo os modos dos pinos
  pinMode(RELEY1, OUTPUT);
  pinMode(RELEY2, OUTPUT);

  Serial.begin(9600);
  Serial.println();
  Serial.print("Iniciando dispositivos");
  mlx.begin();
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

}
//////////////////////////////////////////////////////////

// funcao principal, o loop /////////////////////////////
void loop() {
dist = lerDistancia();
  while ( !( MINDIST < dist && dist < MAXDIST ) ){
    dist = lerDistancia();
    Serial.println("Se aproxime ao sensor.");
  }
  delay(1000);
    for (loopReadTemp = 0; loopReadTemp < 5; loopReadTemp++) {

    sumTemp = 0;

    Serial.println("-------------------------------");
    
    for(int i = 0; i < TOTALREADTEMP; i++){
      readTemp[i] = lerTemp();
      sumTemp += readTemp[i];
      Serial.print("Temperatura n");
      Serial.print(i+1);
      Serial.print(": ");
      Serial.println(readTemp[i]);
      delay(100);
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
  }
  if (loopReadTemp == 5) Serial.println("Sua medição falhou!");

  offRedLed();
  offGreenLed();
  delay(3000);
}
//////////////////////////////////////////////////////////
