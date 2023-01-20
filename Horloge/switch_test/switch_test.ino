byte testpins[8] = {
  PIN_PA4,
  PIN_PA5,
  PIN_PB0,
  PIN_PB1,
  PIN_PB2,
  PIN_PC0,
  PIN_PC1,
  PIN_PC2
};

byte ct = 0;

void setup() {
  pinMode(PIN_PA1, INPUT);
  for(byte x = 0; x > 8; x++) {
    pinMode(testpins[x], OUTPUT);
    digitalWrite(testpins[x], LOW);
  }
}

void loop() {
  if(digitalRead(PIN_A1)) {
    digitalWrite(testpins[ct], LOW);
    ct++;
    if(ct == 8)
      ct = 0;
    digitalWrite(testpins[ct], HIGH);

    delay(500);
  }
}
