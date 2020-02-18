#include "Arduino.h"
#include "BLEDevice.h"

/*
 *UUID do serviço ao qual vamos nos conectar, aqui busca-se um serviço em todos os
   dispositivos
*/
static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b"); 

/*
 *UUID da característica contida no serviço acima que queremos nos conectar
*/
static BLEUUID    charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8"); 

/*
 *Flags
*/
static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
unsigned long cont = 0;

/*
 *Ponteiro pra BLERemoteCharacteristic que representa a característica criada no
  server aqui no client
*/
static BLERemoteCharacteristic* pRemoteCharacteristic;

/*
 *Ponteiro para BLEAdvertisedDevice que representa o dispositivo ao qual estamos
   nos conectando (profile)
*/
static BLEAdvertisedDevice* myDevice;

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

/*
 *Função que primeiro tenta realizar a conexão com o server, depois com a 
   característica, ao partir das UUID's definidas anteriormente, além de
   identificar se houve falha ou não em algm processo de conexão
*/
bool connectToServer() {
  Serial.print("Formando conexão com: ");

  /*
  *Acessa o nome do dispositivo(profile (TESTE))
  */
  Serial.println(myDevice->getAddress().toString().c_str());
    
 /*
 *Cria um client representado pela variável pClient
 */
  BLEClient* pClient = BLEDevice::createClient();

 /*
 *Notifica da criação do client
 */
  Serial.println(" - Client criado");

 /*
 *Conecra o client ao server
 */
 pClient->setClientCallbacks(new MyClientCallback());

 /*
 *Conecta o client criado com o dispositivo
 */
 pClient->connect(myDevice); 
 
 /*
 *Notifica sobre a conexão com o service
 */  
 Serial.println(" - Conectado ao dispositivo");
 
 /*
  *Obtém a referência do service remoto que estamos procurando
 */    
 BLERemoteService* pRemoteService = pClient->getService(serviceUUID);

 /*
 *Caso não tenha encontrado tal referência/ponteiro
 */  
 if (pRemoteService == nullptr) {
    /*
     *Adverte da falha em achar o service com UUID:
    */
    Serial.print("Falha em achar o serviço com UUID: ");

    /*
      *UUID do service do qual não foi possível se conectar
    */
    Serial.println(serviceUUID.toString().c_str());

    /*
     *Desconecta o client
    */
    pClient->disconnect();
    
    /*
     *Retorna nível lógico baixo
    */
    return false;
  }

    /*
     *Caso o ponteiro não seja nulo
     *Notifica que o service foi encontrado
    */    
    Serial.println(" - Service encontrado");
    
    /*
     *Obtém a referência da característica remota que estamos procurando
     *Procura pela característica
    */
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);

    /*
     *Teste se a referÊncia da característica foi encontrada
    */
    if (pRemoteCharacteristic == nullptr) {
      /*
       *Avisa da falha ao encontrar a característica, 
      */
      Serial.print("Falha ao encontrar a característica com UUID: ");
      Serial.println(charUUID.toString().c_str());

      /*
       *Desconecta o client
      */
      pClient->disconnect();

      /*
       *Retorna nível lógico baixo
      */
      return false;
    }


    /*
     *Caso tenha enontrado o UUID da característica
    */
    Serial.println(" - Encontramos a característica");

    
    /*
     *canRead retorna uma variável booleana que retorna 1 para
      o caso de o valor da característica pode ser lido e 0 pro contrário
    */
    if(pRemoteCharacteristic->canRead()) {
      /*
       *Printa o valor da característica no monitor serial
      */
      std::string value = pRemoteCharacteristic->readValue();
      Serial.print("O valor da característica é: ");
      Serial.println(value.c_str());
    }
    
    /*
     *Retorna valor verdadeiro
     *Pode se conectar com um server, uma característica, e mostrou o valor
      dessa característica no monitor serial
    */
    connected = true;
}
/*
 *Escaneia por BLE servers e se conecta ao primeiro que encontrar a UUID buscada
 */


/*
 *Callback desconhecido
*/
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  /*
   * Chama por cada advertising BLE server
  */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // Encontramos o dispositivo, vamos ver se ele contém o serviço que estamos procurando
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Encontramos o server
  } // onResult
}; // MyAdvertisedDeviceCallbacks


void setup() {
  /*
   *Inicializa o monitor serial e printa a frase
  */
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial.println("Inicializando client BLE");

  /*
   *Inicializa o dispositivo
  */
  BLEDevice::init("");

  /*
   *Tente novamente escanear e seta o callback que qeuremos usar para serms informados
    quando detectarmos um novo dispositivo
   *Especifique isso quando ativarmos o escabeaento e comece a escanear por 5 segundos
  */

  // Comece o scan
  BLEScan* pBLEScan = BLEDevice::getScan();

  //defina o callback que qeuremos usar para sermos informados quando se detectou
  //um novo dispositivo
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());

  // Especifique que queremos uma varredura ativa e que dure 5 segundos
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
} 

void loop() {
  /*
   *Caso a flag "doConnect" seja verdadeira, então nós escaneamos e encontramos o
    BLE server que se deseja conectar
   *Agora que estamos conectado a ele, chamamos a função escrita anteriormente 
    connectToServer, que ativa a flag connected
  */
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("Agora estamos conectados ao BLE server");
    } else {
      Serial.println("Falha ao se conectar ao server, não há nada que se possa fazer");
    }
    doConnect = false;
  }

  /*
   *Caso tenha se conectado ao BLEServer, atualizr o valor da caracterítica cada vez que nós 
    alcançarmos o valor de tempo
  */
  if (connected) {
    std::string value = pRemoteCharacteristic->readValue();
    Serial.print(" - Valor da característica: ");
    Serial.println(value.c_str());
    if((millis()-cont)>10){
      Serial2.write(value.c_str());  
    }
    

    
     }else if(doScan){
    /*
     *Isto é apenas um exemplo para começar a escanear depois de desonectado, muito provavelmente
      é o melhor jeito de se fazer isso com arduino
    */
    BLEDevice::getScan()->start(0);  
    
  }
  delay(1000);
}