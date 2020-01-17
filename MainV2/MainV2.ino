#include <SSD1306Wire.h>//biblioteca do Oled

#define pin_relay 13//5
#define pin_solo 15//36
#define scl 5//22
#define sda 18//21

#define HUM_SOLO_MIN 20.1
#define HUM_AR_MIN 60.1
#define TEMP_AR_MAX 25.1

float hum_porcentagem, hum_ar,temp_ar;
int erro;
String mensagem = "";

void medeUmidadeDoSoloSalvaEmPorcentagem () {
  umidade_solo_porcentagem = map (analogRead (pin_solo),4095,1000,0,100);
}

void leituraSensorHumidadeEtemperaturaDoAr(){
  delay(2000);

  humidade = dht.readHumidity();
  // Read temperature as Celsius (the default)
  temperatura = dht.readTemperature();  

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidade) || isnan(temperatura)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidade: "));
  Serial.print(humidade);
  Serial.print(F("%  Temperatura: "));
  Serial.print(temperatura);
  Serial.println(F("°C "));
}

void MostraParaUsuario() {//faz exatamente oq o nome fala =)
  if (erro) {
    display.clear();
    display.setFont(ArialMT_Plain_24);
    display.drawRect(0, 0, 128, 64);
    display.drawString(2,10,"  Irrigando\n");
    display.display();
    delay (100);
    display.setFont(ArialMT_Plain_16);
    display.drawString(8,37," umidade baixa\n");
    display.display();//faz todos os comandos aparecerem no oled
  }else {
    display.clear();
    display.setFont(ArialMT_Plain_24);
    display.drawRect(0, 0, 128, 64);
    display.drawString(0,5,"  Aqumatic  \n  Saudável\n");
    display.display();
  }
}

void verificaErros () {
  if (umidade_solo_porcentagem <= umidade_solo_minima) {//valor de humidade do solo que a planta acha ruim
    erro = 1;
  }else {
    erro = 0;
  }
}

SSD1306Wire  display(0x3c, sda, scl);

void setup() {
  pinMode(pin_solo,INPUT);
  pinMode(pin_relay, OUTPUT);
  display.init();//inicia o display do oled
  display.setFont(ArialMT_Plain_24);
}

void loop() {
  // put your main code here, to run repeatedly:

}
