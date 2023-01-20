byte testpins[4] = {
  PIN_PA4,
  PIN_PA5,
  PIN_PB1,
  PIN_PB2
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
        pinMode(PIN_PB1, INPUT);
        digitalWrite(PIN_PB1, LOW);
        pinMode(PIN_PB2, INPUT);
        digitalWrite(PIN_PB2, LOW);
        
        pinMode(PIN_PA5, OUTPUT);
        pinMode(PIN_PA4, OUTPUT);
        digitalWrite(PIN_PA5, HIGH);
        digitalWrite(PIN_PA4, LOW);
        break;
        
      case 1:
        pinMode(PIN_PB1, INPUT);
        digitalWrite(PIN_PB1, LOW);
        pinMode(PIN_PA5, INPUT);
        digitalWrite(PIN_PA5, LOW);
        
        pinMode(PIN_PB2, OUTPUT);
        pinMode(PIN_PA4, OUTPUT);
        digitalWrite(PIN_PA4, HIGH);
        digitalWrite(PIN_PB2, LOW);
        break;
        
      case 2:
        pinMode(PIN_PA4, INPUT);
        digitalWrite(PIN_PA4, LOW);
        pinMode(PIN_PA5, INPUT);
        digitalWrite(PIN_PA5, LOW);
        
        pinMode(PIN_PB2, OUTPUT);
        pinMode(PIN_PB1, OUTPUT);
        digitalWrite(PIN_PB2, HIGH);
        digitalWrite(PIN_PB1, LOW);
        break;
        
      case 3:
        pinMode(PIN_PB1, INPUT);
        digitalWrite(PIN_PB1, LOW);
        pinMode(PIN_PB2, INPUT);
        digitalWrite(PIN_PB2, LOW);
        
        pinMode(PIN_PA5, OUTPUT);
        pinMode(PIN_PA4, OUTPUT);
        digitalWrite(PIN_PA4, HIGH);
        digitalWrite(PIN_PA5, LOW);
        break;
        
      case 4:
        pinMode(PIN_PB1, INPUT);
        digitalWrite(PIN_PB1, LOW);
        pinMode(PIN_PA5, INPUT);
        digitalWrite(PIN_PA5, LOW);
        
        pinMode(PIN_PB2, OUTPUT);
        pinMode(PIN_PA4, OUTPUT);
        digitalWrite(PIN_PB2, HIGH);
        digitalWrite(PIN_PA4, LOW);
        break;
        
      case 5:
        pinMode(PIN_PA4, INPUT);
        digitalWrite(PIN_PA4, LOW);
        pinMode(PIN_PA5, INPUT);
        digitalWrite(PIN_PA5, LOW);
        
        pinMode(PIN_PB1, OUTPUT);
        pinMode(PIN_PB2, OUTPUT);
        digitalWrite(PIN_PB1, HIGH);
        digitalWrite(PIN_PB2, LOW);
        break;
        
      case 6:
        pinMode(PIN_PB2, INPUT);
        digitalWrite(PIN_PB2, LOW);
        pinMode(PIN_PA5, INPUT);
        digitalWrite(PIN_PA5, LOW);
        
        pinMode(PIN_PB1, OUTPUT);
        pinMode(PIN_PA4, OUTPUT);
        digitalWrite(PIN_PA4, HIGH);
        digitalWrite(PIN_PB1, LOW);
        break;
        
      case 7:
        pinMode(PIN_PB2, INPUT);
        digitalWrite(PIN_PB2, LOW);
        pinMode(PIN_PA5, INPUT);
        digitalWrite(PIN_PA5, LOW);
        
        pinMode(PIN_PB1, OUTPUT);
        pinMode(PIN_PA4, OUTPUT);
        digitalWrite(PIN_PB1, HIGH);
        digitalWrite(PIN_PA4, LOW);
        break;
        
      case 8:
        pinMode(PIN_PB1, INPUT);
        digitalWrite(PIN_PB1, LOW);
        pinMode(PIN_PA4, INPUT);
        digitalWrite(PIN_PA4, LOW);
        
        pinMode(PIN_PA5, OUTPUT);
        pinMode(PIN_PB2, OUTPUT);
        digitalWrite(PIN_PA5, HIGH);
        digitalWrite(PIN_PB2, LOW);
        break;
        
      case 9:
        pinMode(PIN_PB1, INPUT);
        digitalWrite(PIN_PB1, LOW);
        pinMode(PIN_PA4, INPUT);
        digitalWrite(PIN_PA4, LOW);
        
        pinMode(PIN_PA5, OUTPUT);
        pinMode(PIN_PB2, OUTPUT);
        digitalWrite(PIN_PB2, HIGH);
        digitalWrite(PIN_PA5, LOW);
        break;
        
      case 10:
        pinMode(PIN_PB2, INPUT);
        digitalWrite(PIN_PB2, LOW);
        pinMode(PIN_PA4, INPUT);
        digitalWrite(PIN_PA4, LOW);
        
        pinMode(PIN_PA5, OUTPUT);
        pinMode(PIN_PB1, OUTPUT);
        digitalWrite(PIN_PA5, HIGH);
        digitalWrite(PIN_PB1, LOW);
        break;
        
      case 11:
        pinMode(PIN_PB2, INPUT);
        digitalWrite(PIN_PB2, LOW);
        pinMode(PIN_PA4, INPUT);
        digitalWrite(PIN_PA4, LOW);
        
        pinMode(PIN_PA5, OUTPUT);
        pinMode(PIN_PB1, OUTPUT);
        digitalWrite(PIN_PB1, HIGH);
        digitalWrite(PIN_PA5, LOW);
        break;
    }

    delay(500);
  }
}
