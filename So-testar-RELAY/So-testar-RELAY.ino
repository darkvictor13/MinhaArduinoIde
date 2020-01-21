#define pin_relay 12

void ligarRelay() {
  digitalWrite(pin_relay, HIGH);
}

void desligarRelay() {
  digitalWrite(pin_relay, LOW);
}

void setup() {
  pinMode(pin_relay, OUTPUT);
}

void loop() {
  ligarRelay();
  delay(2000);
  desligarRelay();
  delay(2000);
}
