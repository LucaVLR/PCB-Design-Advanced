byte testpins[4] = {
  PIN_PC2,
  PIN_PB0,
  PIN_PC0,
  PIN_PC1
};

byte ct = 0;

void setup() {
  pinMode(PIN_PA1, INPUT);
  for(byte x = 0; x > 4; x++) {
    pinMode(testpins[x], INPUT);
    digitalWrite(testpins[x], LOW);
  }
}

void loop() {
  if(digitalRead(PIN_A1)) {
    for(byte x = 0; x > 4; x++) {
      pinMode(testpins[x], INPUT);
      digitalWrite(testpins[x], LOW);
    }

    ct++;
    if(ct == 12)
      ct = 0;
      
    switch(ct) {
      case 0:
        pinMode(PIN_PC0, INPUT);
        digitalWrite(PIN_PC0, LOW);
        pinMode(PIN_PC1, INPUT);
        digitalWrite(PIN_PC1, LOW);
        
        pinMode(PIN_PB0, OUTPUT);
        pinMode(PIN_PC2, OUTPUT);
        digitalWrite(PIN_PB0, HIGH);
        digitalWrite(PIN_PC2, LOW);
        break;
        
      case 1:
        pinMode(PIN_PC0, INPUT);
        digitalWrite(PIN_PC0, LOW);
        pinMode(PIN_PB0, INPUT);
        digitalWrite(PIN_PB0, LOW);
        
        pinMode(PIN_PC1, OUTPUT);
        pinMode(PIN_PC2, OUTPUT);
        digitalWrite(PIN_PC2, HIGH);
        digitalWrite(PIN_PC1, LOW);
        break;
        
      case 2:
        pinMode(PIN_PC2, INPUT);
        digitalWrite(PIN_PC2, LOW);
        pinMode(PIN_PB0, INPUT);
        digitalWrite(PIN_PB0, LOW);
        
        pinMode(PIN_PC1, OUTPUT);
        pinMode(PIN_PC0, OUTPUT);
        digitalWrite(PIN_PC1, HIGH);
        digitalWrite(PIN_PC0, LOW);
        break;
        
      case 3:
        pinMode(PIN_PC0, INPUT);
        digitalWrite(PIN_PC0, LOW);
        pinMode(PIN_PC1, INPUT);
        digitalWrite(PIN_PC1, LOW);
        
        pinMode(PIN_PB0, OUTPUT);
        pinMode(PIN_PC2, OUTPUT);
        digitalWrite(PIN_PC2, HIGH);
        digitalWrite(PIN_PB0, LOW);
        break;
        
      case 4:
        pinMode(PIN_PC0, INPUT);
        digitalWrite(PIN_PC0, LOW);
        pinMode(PIN_PB0, INPUT);
        digitalWrite(PIN_PB0, LOW);
        
        pinMode(PIN_PC1, OUTPUT);
        pinMode(PIN_PC2, OUTPUT);
        digitalWrite(PIN_PC1, HIGH);
        digitalWrite(PIN_PC2, LOW);
        break;
        
      case 5:
        pinMode(PIN_PC2, INPUT);
        digitalWrite(PIN_PC2, LOW);
        pinMode(PIN_PB0, INPUT);
        digitalWrite(PIN_PB0, LOW);
        
        pinMode(PIN_PC0, OUTPUT);
        pinMode(PIN_PC1, OUTPUT);
        digitalWrite(PIN_PC0, HIGH);
        digitalWrite(PIN_PC1, LOW);
        break;
        
      case 6:
        pinMode(PIN_PC1, INPUT);
        digitalWrite(PIN_PC1, LOW);
        pinMode(PIN_PB0, INPUT);
        digitalWrite(PIN_PB0, LOW);
        
        pinMode(PIN_PC0, OUTPUT);
        pinMode(PIN_PC2, OUTPUT);
        digitalWrite(PIN_PC2, HIGH);
        digitalWrite(PIN_PC0, LOW);
        break;
        
      case 7:
        pinMode(PIN_PC1, INPUT);
        digitalWrite(PIN_PC1, LOW);
        pinMode(PIN_PB0, INPUT);
        digitalWrite(PIN_PB0, LOW);
        
        pinMode(PIN_PC0, OUTPUT);
        pinMode(PIN_PC2, OUTPUT);
        digitalWrite(PIN_PC0, HIGH);
        digitalWrite(PIN_PC2, LOW);
        break;
        
      case 8:
        pinMode(PIN_PC0, INPUT);
        digitalWrite(PIN_PC0, LOW);
        pinMode(PIN_PC2, INPUT);
        digitalWrite(PIN_PC2, LOW);
        
        pinMode(PIN_PB0, OUTPUT);
        pinMode(PIN_PC1, OUTPUT);
        digitalWrite(PIN_PB0, HIGH);
        digitalWrite(PIN_PC1, LOW);
        break;
        
      case 9:
        pinMode(PIN_PC0, INPUT);
        digitalWrite(PIN_PC0, LOW);
        pinMode(PIN_PC2, INPUT);
        digitalWrite(PIN_PC2, LOW);
        
        pinMode(PIN_PB0, OUTPUT);
        pinMode(PIN_PC1, OUTPUT);
        digitalWrite(PIN_PC1, HIGH);
        digitalWrite(PIN_PB0, LOW);
        break;
        
      case 10:
        pinMode(PIN_PC1, INPUT);
        digitalWrite(PIN_PC1, LOW);
        pinMode(PIN_PC2, INPUT);
        digitalWrite(PIN_PC2, LOW);
        
        pinMode(PIN_PB0, OUTPUT);
        pinMode(PIN_PC0, OUTPUT);
        digitalWrite(PIN_PB0, HIGH);
        digitalWrite(PIN_PC0, LOW);
        break;
        
      case 11:
        pinMode(PIN_PC1, INPUT);
        digitalWrite(PIN_PC1, LOW);
        pinMode(PIN_PC2, INPUT);
        digitalWrite(PIN_PC2, LOW);
        
        pinMode(PIN_PB0, OUTPUT);
        pinMode(PIN_PC0, OUTPUT);
        digitalWrite(PIN_PC0, HIGH);
        digitalWrite(PIN_PB0, LOW);
        break;
    }

    delay(500);
  }
}
