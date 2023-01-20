#include <Wire.h>
#include <twi.h>
#include <twi_pins.h>

byte sensors = 0; // Holds states of all 8 IR sensors as 1 or 0

void setup() {
  PORTC.DIR = PIN2_bp | PIN3_bp | PIN4_bp | PIN5_bp;  // Set PC2-5 input
  PORTB.DIR = PIN4_bp | PIN5_bp | PIN6_bp | PIN7_bp;  // Set PB4-7 input
  
  Wire.begin(0x65);
  Wire.onRequest(WireRequest);

  Serial.begin(115200);
}

void loop() {
  if(Serial.available()) {
    while(Serial.available())
      Serial.read();  // Actual data in serial buffer is redundant, this is purely for requests

    Serial.write(sensors);
  }

  // Read state of the sensor and shift it into its relevant place in the buffer variable
  sensors = 0b00000001 & (PORTC.IN & PIN2_bm);
  sensors = 0b00000010 & ((PORTC.IN & PIN3_bm) << 1);
  sensors = 0b00000100 & ((PORTC.IN & PIN4_bm) << 2);
  sensors = 0b00001000 & ((PORTC.IN & PIN5_bm) << 3);
  sensors = 0b00010000 & ((PORTB.IN & PIN4_bm) << 4);
  sensors = 0b00100000 & ((PORTB.IN & PIN5_bm) << 5);
  sensors = 0b01000000 & ((PORTB.IN & PIN6_bm) << 6);
  sensors = 0b10000000 & ((PORTB.IN & PIN7_bm) << 7);
}

void WireRequest() {
  Wire.write(sensors);
}