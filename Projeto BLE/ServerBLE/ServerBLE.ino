#include "Arduino.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
//int2str
#include <sstream>
#include <string.h>
using namespace std;

/*
 *Define o ponteiro pServer nulo como variável global
 *Variável que represrnta o device
*/
BLEServer* pServer = NULL;

/*
 *Define o ponteiro pCharacteristic nulo como variável global
 *Variável que representa a característica
*/
BLECharacteristic* pCharacteristic = NULL;

/*
 *Flags
*/
bool deviceConnected = false;
bool oldDeviceConnected = false;
int value = 0;

/*
 *UUID do serviço e de sua característica
*/
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

/*
 *Callback
*/
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};



void setup() {
  Serial.begin(115200);

  /*
   *Cria o dispositivo 
  */
  BLEDevice::init("ESP32_1");

  /*
   Cria o server e declara o callback dentro do mesmo
  */
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  /*
   *Cria o serviço ao partir da variável pServer global anterior
  */
  BLEService *pService = pServer->createService(SERVICE_UUID);

  /*
   *Cria a característica com diferentes propiedades
   *Ao partir do serviço criado anteriormente
  */
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  /*
   *Aqui cria-se um descriptor, perigo
  */
  pCharacteristic->addDescriptor(new BLE2902());

  /*
   *Inicializa o serviço
  */
  pService->start();

  /*
   *Inicializa o advertising do service
  */
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);

  /*
   *Aqui eu mudei de false pra true
  */
  pAdvertising->setScanResponse(true); //Cuidado
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Esperando a notificação de conexão com o client");//Esperando a notificação de conexão do client
}

void loop() {
  /*
   *Esta flag torna-se verdadeira na classe call back
  */
  if (deviceConnected) {
    /*
     *Seta um valor para a característica
     *Fico gerando valores para enviar ao client
    */
    /*
     *Tenho de transformar este contador value em string para mandar ao client
     *int2str
    */
    int number=value;
    stringstream strs;
    strs << number;
    string temp_str = strs.str();
    char* newValue = (char*) temp_str.c_str();

    pCharacteristic->setValue(newValue);
    pCharacteristic->notify();
    value++;

    if (value == 10){
      value = 0;
    }
    /*
     *Bluetooth stack will go into congestion, if too many packets are sent, 
      in 6 hours test i was able to go as low as 3ms
    */
    delay(1000); 
  }

  /*
   *oldDeviceConnected não tem valor verdadeiro na primeira iteração
  */
  if (!deviceConnected && oldDeviceConnected) {
    delay(5000); // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }

  /*
   *?
  */
  if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
  }
}