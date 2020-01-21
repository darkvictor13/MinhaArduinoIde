#define PIN_SOLO 15
int umidade_do_solo;

void iniciaSensorSolo() {
  pinMode(PIN_SOLO, INPUT);
}

void medeUmidadeDoSolo() {
  umidade_do_solo = map (analogRead (PIN_SOLO), 4095, 1000, 0, 100);
  delay(2000);
}

void setup() {
  Serial.begin(9600);
  iniciaSensorSolo();
}

void loop() {
  medeUmidadeDoSolo();
  Serial.println(umidade_do_solo);
}
