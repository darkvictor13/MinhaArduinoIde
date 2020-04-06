#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define botao1 13
#define botao2 2

#define SINAL_POSITIVO 'A'
#define SINAL_NEGATIVO 'B'
const uint8_t sinal_positivo = 65;
const uint8_t sinal_negativo = 66;

BluetoothSerial SerialBT;

void enviarSinal(const uint8_t ch) {
  if (SerialBT.available()) {
      SerialBT.write(&ch, 1);
  }
  delay(2000);
}

void setup() {
  Serial.begin(115200);
  pinMode(botao1, INPUT);
  pinMode(botao2, INPUT);
  SerialBT.begin("Bracelete"); //Bluetooth device name
  Serial.println("Bracelete iniciado agora pereie ele ao celular!");
}

void loop() {
  //Serial.println("Cheguei");
  if (digitalRead(botao1) == HIGH) {
    Serial.println("Botao verde apertado!");
    enviarSinal(sinal_positivo);
  }
  if (digitalRead(botao2) == HIGH) {
    Serial.println("Botao vermelho apertado!");
    enviarSinal(sinal_negativo);
  }
}
