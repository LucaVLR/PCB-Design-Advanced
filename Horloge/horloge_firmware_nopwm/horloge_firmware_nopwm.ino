#include <avr/sleep.h> 

volatile byte sec_ct = 0;
volatile byte min_ct = 0;
volatile byte hr_ct = 12;
volatile bool displayflag = false;

#define pressTime 2000   // Time in milliseconds that makes a button press a "long" press
#define BUTTON_PRESSED PORTA.IN & PIN1_bm // Used to check if PA1 = HIGH, therefore button is pressed
#define LONG_PRESS 2
#define PRESS 1
#define NO_PRESS 0

#define displayTime 5000 // Time in milliseconds to display watch face before sleep

void setup() {
  byte calibrate_state = 0; // Calibration state: 0 = hours, 1 = minutes, 2 = done
  
  // Button setup
  PORTA.DIR = PIN1_bp;  // Set PA1 input
  clearLEDs();

  // Calibration mode - Only entered once on powerup after battery change
  while(calibrate_state < 2) {
    switch(calibrate_state) {
      case 0:
        writeHours(hr_ct);
        
        switch(buttonPress()) {
          case NO_PRESS:
            break;
          case PRESS:
            hr_ct++;
            if(hr_ct == 13)
              hr_ct = 1;
            break;
          case LONG_PRESS:
            calibrate_state++;
            break;
        }
        break;

      case 1:
        writeMinutes(min_ct);

        switch(buttonPress()) {
          case NO_PRESS:
            break;
          case PRESS:
            min_ct +=5;
            if(min_ct == 60)
              min_ct = 0;
            break;
          case LONG_PRESS:
            calibrate_state++;
            break;
        }
        break;
    }
  }

  clearLEDs();

  // Button interrupt setup
  PORTA.PIN1CTRL = PORT_ISC_BOTHEDGES_gc; // enable interrupt on change (allows wake-up function in standby)
  
  // RTC setup
  RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;    // 32.768kHz Internal Crystal Oscillator (INT32K)
  while (RTC.STATUS > 0);               // Wait for all registers to be synchronized
  RTC.PER = 1000;                   // Set period for delay
  RTC.INTCTRL |= RTC_OVF_bm;            // Enable overflow Interrupt which will trigger ISR
  RTC.CTRLA = RTC_PRESCALER_DIV32_gc    // 32768 / 32 = 1024 (sec) ~ 1 ms
  | RTC_RTCEN_bm                        // Enable: enabled 
  | RTC_RUNSTDBY_bm;                    // Run In Standby: enabled

  set_sleep_mode(SLEEP_MODE_STANDBY);  // Set sleep mode to STANDBY mode
  sleep_enable();
}

void loop() {
  if(displayflag) {  // if PA1 interrupt
    writeMinutes(min_ct);
    writeHours(hr_ct);

    uint32_t starttime = millis();
    do{}while((millis() - starttime) < displayTime);

    clearLEDs();
    displayflag = false;
  }
  sleep_cpu();
}

ISR(RTC_CNT_vect) {
  RTC.INTFLAGS = RTC_OVF_bm;            // Clear flag by writing '1':
  sec_ct++;
  
  if(sec_ct == 60) {
    min_ct++;
    sec_ct = 0;
    if(min_ct == 60) {
      hr_ct++;
      min_ct = 0;
      if(hr_ct == 13)
        hr_ct = 1;
    }
  }
}

ISR(PORTA_PORT_vect) {
  PORTA.PIN1CTRL &= ~PORT_ISC_gm; // disable interrupt
  displayflag = true;

  PORTA.INTFLAGS = PIN1_bm; // Clear flag
  PORTA.PIN1CTRL = PORT_ISC_BOTHEDGES_gc; // enable interrupt on change
}

byte buttonPress(){
  uint32_t startTime = 0;

  if(BUTTON_PRESSED) {
    startTime = millis();   // Start Non-Interrupting Timer

    while(BUTTON_PRESSED){
      if((millis() - startTime) >= pressTime)
        return LONG_PRESS; // Return long press value
    }
    return PRESS; // Pressed, but no long press
  }
  return NO_PRESS; // Button was not pressed during function runtime
}

void writeMinutes(byte minutes) {
  switch(minutes) {
    case 0 ... 4:
      PORTC.DIR = PIN0_bp | PIN1_bp;    // Set C0, C1 input
      PORTC.OUTSET = PIN0_bp | PIN1_bp; // Write PC0 PC1 low, high impedance tri-state

      PORTB.DIR = PIN0_bm;              // Set B0 output
      PORTC.DIR = PIN2_bm;              // Set C2 output

      PORTB.OUTSET = PIN0_bm; // Write PB0 High - Atomic operation taking 2-3 words, 2-3 clocks
      PORTC.OUTCLR = PIN2_bm; // Write PC2 Low - Atomic operation taking 2-3 words, 2-3 clocks
      break;
      
    case 5 ... 9:
      PORTC.DIR = PIN0_bp | PIN2_bp;
      PORTC.OUTSET = PIN0_bp | PIN2_bp;

      PORTB.DIR = PIN0_bm;              // Set B0 output
      PORTC.DIR = PIN1_bm;              // Set C1 output

      PORTB.OUTSET = PIN0_bm; // Write PB0 High - Atomic operation taking 2-3 words, 2-3 clocks
      PORTC.OUTCLR = PIN1_bm; // Write PC1 Low - Atomic operation taking 2-3 words, 2-3 clocks
      break;
      
    case 10 ... 14:
      PORTC.DIR = PIN1_bp | PIN2_bp;    // Set C1, C2 input
      PORTC.OUTSET = PIN1_bp | PIN2_bp; // Write PC1 PC2 low, high impedance tri-state

      PORTB.DIR = PIN0_bm;              // Set B0 output
      PORTC.DIR = PIN0_bm;              // Set C0 output

      PORTB.OUTSET = PIN0_bm; // Write PB0 High - Atomic operation taking 2-3 words, 2-3 clocks
      PORTC.OUTCLR = PIN0_bm; // Write PC0 Low - Atomic operation taking 2-3 words, 2-3 clocks
      break;
      
    case 15 ... 19:
      PORTC.DIR = PIN0_bp | PIN1_bp;    // Set C0, C1 input
      PORTC.OUTSET = PIN0_bp | PIN1_bp; // Write PC0 PC1 low, high impedance tri-state

      PORTB.DIR = PIN0_bm;              // Set B0 output
      PORTC.DIR = PIN2_bm;              // Set C2 output

      PORTC.OUTSET = PIN2_bm; // Write PC2 High - Atomic operation taking 2-3 words, 2-3 clocks
      PORTB.OUTCLR = PIN0_bm; // Write PB0 Low - Atomic operation taking 2-3 words, 2-3 clocks
      break;
      
    case 20 ... 24:
      PORTC.DIR = PIN0_bp;    // Set C0 input
      PORTB.DIR = PIN0_bp;    // Set B0 input
      PORTB.OUTSET = PIN0_bp; // Write PB0 low, high impedance tri-state
      PORTC.OUTSET = PIN0_bp; // Write PC0 low, high impedance tri-state

      PORTC.DIR = PIN1_bm | PIN2_bm;    // Set PC1, PC2 output

      PORTC.OUTSET = PIN2_bm; // Write PC2 High - Atomic operation taking 2-3 words, 2-3 clocks
      PORTC.OUTCLR = PIN1_bm; // Write PC1 Low - Atomic operation taking 2-3 words, 2-3 clocks
      break;
      
    case 25 ... 29:
      PORTC.DIR = PIN1_bp;    // Set C1 input
      PORTB.DIR = PIN0_bp;    // Set B0 input
      PORTB.OUTSET = PIN0_bp; // Write PB0 low, high impedance tri-state
      PORTC.OUTSET = PIN1_bp; // Write PC1 low, high impedance tri-state

      PORTC.DIR = PIN0_bm | PIN2_bm;    // Set PC0, PC2 output

      PORTC.OUTSET = PIN2_bm; // Write PC2 High - Atomic operation taking 2-3 words, 2-3 clocks
      PORTC.OUTCLR = PIN0_bm; // Write PC0 Low - Atomic operation taking 2-3 words, 2-3 clocks
      break;
      
    case 30 ... 34:
      PORTC.DIR = PIN0_bp | PIN2_bp;    // Set C0, C2 input
      PORTC.OUTSET = PIN0_bp | PIN2_bp; // Write PC0 PC2 low, high impedance tri-state

      PORTB.DIR = PIN0_bm;              // Set B0 output
      PORTC.DIR = PIN1_bm;              // Set C1 output

      PORTC.OUTSET = PIN1_bm; // Write PC1 High - Atomic operation taking 2-3 words, 2-3 clocks
      PORTB.OUTCLR = PIN0_bm; // Write PB0 Low - Atomic operation taking 2-3 words, 2-3 clocks
      break;
      
    case 35 ... 39:
      PORTC.DIR = PIN0_bp;    // Set C0 input
      PORTB.DIR = PIN0_bp;    // Set B0 input
      PORTB.OUTSET = PIN0_bp; // Write PB0 low, high impedance tri-state
      PORTC.OUTSET = PIN0_bp; // Write PC0 low, high impedance tri-state

      PORTC.DIR = PIN1_bm | PIN2_bm;    // Set PC1, PC2 output

      PORTC.OUTSET = PIN1_bm; // Write PC1 High - Atomic operation taking 2-3 words, 2-3 clocks
      PORTC.OUTCLR = PIN2_bm; // Write PC2 Low - Atomic operation taking 2-3 words, 2-3 clocks
      break;
      
    case 40 ... 44:
      PORTC.DIR = PIN2_bp;    // Set C2 input
      PORTB.DIR = PIN0_bp;    // Set B0 input
      PORTB.OUTSET = PIN0_bp; // Write PB0 low, high impedance tri-state
      PORTC.OUTSET = PIN0_bp; // Write PC0 low, high impedance tri-state

      PORTC.DIR = PIN0_bm | PIN1_bm;    // Set PC0, PC1 output

      PORTC.OUTSET = PIN1_bm; // Write PC1 High - Atomic operation taking 2-3 words, 2-3 clocks
      PORTC.OUTCLR = PIN0_bm; // Write PC0 Low - Atomic operation taking 2-3 words, 2-3 clocks
      break;
      
    case 45 ... 49:
      PORTC.DIR = PIN1_bp | PIN2_bp;    // Set C1, C2 input
      PORTC.OUTSET = PIN1_bp | PIN2_bp; // Write PC1 PC2 low, high impedance tri-state

      PORTB.DIR = PIN0_bm;              // Set B0 output
      PORTC.DIR = PIN0_bm;              // Set C0 output

      PORTC.OUTSET = PIN0_bm; // Write PC0 High - Atomic operation taking 2-3 words, 2-3 clocks
      PORTB.OUTCLR = PIN0_bm; // Write PB0 Low - Atomic operation taking 2-3 words, 2-3 clocks
      break;
      
    case 50 ... 54:
      PORTC.DIR = PIN1_bp;    // Set C1 input
      PORTB.DIR = PIN0_bp;    // Set B0 input
      PORTB.OUTSET = PIN0_bp; // Write PB0 low, high impedance tri-state
      PORTC.OUTSET = PIN1_bp; // Write PC1 low, high impedance tri-state

      PORTC.DIR = PIN0_bm | PIN2_bm;    // Set PC0, PC2 output

      PORTC.OUTSET = PIN0_bm; // Write PC0 High - Atomic operation taking 2-3 words, 2-3 clocks
      PORTC.OUTCLR = PIN2_bm; // Write PC2 Low - Atomic operation taking 2-3 words, 2-3 clocks
      break;
      
    case 55 ... 59:
      PORTC.DIR = PIN2_bp;    // Set C2 input
      PORTB.DIR = PIN0_bp;    // Set B0 input
      PORTB.OUTSET = PIN0_bp; // Write PB0 low, high impedance tri-state
      PORTC.OUTSET = PIN0_bp; // Write PC0 low, high impedance tri-state

      PORTC.DIR = PIN0_bm | PIN1_bm;    // Set PC0, PC1 output

      PORTC.OUTSET = PIN0_bm; // Write PC0 High - Atomic operation taking 2-3 words, 2-3 clocks
      PORTC.OUTCLR = PIN1_bm; // Write PC1 Low - Atomic operation taking 2-3 words, 2-3 clocks
      break;
  }
}

void writeHours(byte hours) {
  switch(hours) {
    case 1:
      PORTB.DIRCLR = PIN1_bm;    // Set B1 input
      PORTA.DIRCLR = PIN4_bm;    // Set A4 input
      PORTB.OUTSET = PIN1_bp; // Write PB1 low, high impedance tri-state
      PORTA.OUTSET = PIN4_bp; // Write PA4 low, high impedance tri-state

      PORTA.DIR = PIN5_bm;  // Set A5 output
      PORTB.DIR = PIN2_bm;  // Set B2 output

      PORTA.OUTSET = PIN5_bm; // Write PA5 High - Atomic operation taking 2-3 words, 2-3 clocks
      PORTB.OUTCLR = PIN2_bm; // Write PB2 Low - Atomic operation taking 2-3 words, 2-3 clocks
      break;

    case 2:
      PORTB.DIRCLR = PIN2_bm;    // Set B2 input
      PORTA.DIRCLR = PIN4_bm;    // Set A4 input
      PORTB.OUTSET = PIN2_bp; // Write PB2 low, high impedance tri-state
      PORTA.OUTSET = PIN4_bp; // Write PA4 low, high impedance tri-state

      PORTA.DIR = PIN5_bm;  // Set A5 output
      PORTB.DIR = PIN1_bm;  // Set B1 output

      PORTA.OUTSET = PIN5_bm; // Write PA5 High - Atomic operation taking 2-3 words, 2-3 clocks
      PORTB.OUTCLR = PIN1_bm; // Write PB1 Low - Atomic operation taking 2-3 words, 2-3 clocks
      break;

    case 3:
      PORTB.DIRCLR = PIN1_bm | PIN2_bm;    // Set B1, B2 input
      PORTB.OUTSET = PIN1_bp | PIN2_bp; // Write PB1 PB2 low, high impedance tri-state

      PORTA.DIR = PIN4_bm | PIN5_bm;              // Set A4 A5 output

      PORTA.OUTSET = PIN4_bm; // Write PA4 High - Atomic operation taking 2-3 words, 2-3 clocks
      PORTA.OUTCLR = PIN5_bm; // Write PA5 Low - Atomic operation taking 2-3 words, 2-3 clocks
      break;

    case 4:
      PORTB.DIRCLR = PIN1_bm;    // Set B1 input
      PORTA.DIRCLR = PIN5_bm;    // Set A5 input
      PORTB.OUTSET = PIN1_bp; // Write PB1 low, high impedance tri-state
      PORTA.OUTSET = PIN5_bp; // Write PA5 low, high impedance tri-state

      PORTA.DIR = PIN4_bm;  // Set A4 output
      PORTB.DIR = PIN2_bm;  // Set B2 output

      PORTA.OUTSET = PIN4_bm; // Write PA4 High - Atomic operation taking 2-3 words, 2-3 clocks
      PORTB.OUTCLR = PIN2_bm; // Write PB2 Low - Atomic operation taking 2-3 words, 2-3 clocks
      break;

    case 5:
      PORTB.DIRCLR = PIN2_bm;    // Set B2 input
      PORTA.DIRCLR = PIN5_bm;    // Set A5 input
      PORTB.OUTSET = PIN2_bp; // Write PB2 low, high impedance tri-state
      PORTA.OUTSET = PIN5_bp; // Write PA5 low, high impedance tri-state

      PORTA.DIR = PIN4_bm;  // Set A4 output
      PORTB.DIR = PIN1_bm;  // Set B1 output

      PORTA.OUTSET = PIN4_bm; // Write PA4 High - Atomic operation taking 2-3 words, 2-3 clocks
      PORTB.OUTCLR = PIN1_bm; // Write PB1 Low - Atomic operation taking 2-3 words, 2-3 clocks
      break;

    case 6:
      PORTB.DIRCLR = PIN1_bm;    // Set B1 input
      PORTA.DIRCLR = PIN4_bm;    // Set A4 input
      PORTB.OUTSET = PIN1_bp; // Write PB1 low, high impedance tri-state
      PORTA.OUTSET = PIN4_bp; // Write PA4 low, high impedance tri-state

      PORTA.DIR = PIN5_bm;  // Set A5 output
      PORTB.DIR = PIN2_bm;  // Set B2 output

      PORTB.OUTSET = PIN2_bm; // Write PB2 High - Atomic operation taking 2-3 words, 2-3 clocks
      PORTA.OUTCLR = PIN5_bm; // Write PA5 Low - Atomic operation taking 2-3 words, 2-3 clocks
      break;

    case 7:
      PORTB.DIRCLR = PIN1_bm;    // Set B1 input
      PORTA.DIRCLR = PIN5_bm;    // Set A5 input
      PORTB.OUTSET = PIN1_bp; // Write PB1 low, high impedance tri-state
      PORTA.OUTSET = PIN5_bp; // Write PA5 low, high impedance tri-state

      PORTA.DIR = PIN4_bm;  // Set A4 output
      PORTB.DIR = PIN2_bm;  // Set B2 output

      PORTB.OUTSET = PIN2_bm; // Write PB2 High - Atomic operation taking 2-3 words, 2-3 clocks
      PORTA.OUTCLR = PIN4_bm; // Write PA4 Low - Atomic operation taking 2-3 words, 2-3 clocks
      break;

    case 8:
      PORTA.DIRCLR = PIN4_bm | PIN5_bm;    // Set A4, A5 input
      PORTA.OUTSET = PIN4_bp | PIN5_bp; // Write PA4 PA5 low, high impedance tri-state

      PORTB.DIR = PIN1_bm | PIN2_bm;    // Set B1 B2 output

      PORTB.OUTSET = PIN2_bm; // Write PB2 High - Atomic operation taking 2-3 words, 2-3 clocks
      PORTB.OUTCLR = PIN1_bm; // Write PB1 Low - Atomic operation taking 2-3 words, 2-3 clocks
      break;

    case 9:
      PORTB.DIRCLR = PIN2_bm;    // Set B2 input
      PORTA.DIRCLR = PIN4_bm;    // Set A4 input
      PORTB.OUTSET = PIN2_bp; // Write PB2 low, high impedance tri-state
      PORTA.OUTSET = PIN4_bp; // Write PA4 low, high impedance tri-state

      PORTA.DIR = PIN5_bm;  // Set A5 output
      PORTB.DIR = PIN1_bm;  // Set B1 output

      PORTB.OUTSET = PIN1_bm; // Write PB1 High - Atomic operation taking 2-3 words, 2-3 clocks
      PORTA.OUTCLR = PIN5_bm; // Write PA5 Low - Atomic operation taking 2-3 words, 2-3 clocks
      break;

    case 10:
      PORTB.DIRCLR = PIN2_bm;    // Set B2 input
      PORTA.DIRCLR = PIN5_bm;    // Set A5 input
      PORTB.OUTSET = PIN2_bp; // Write PB2 low, high impedance tri-state
      PORTA.OUTSET = PIN5_bp; // Write PA5 low, high impedance tri-state

      PORTA.DIR = PIN4_bm;  // Set A4 output
      PORTB.DIR = PIN1_bm;  // Set B1 output

      PORTB.OUTSET = PIN1_bm; // Write PB1 High - Atomic operation taking 2-3 words, 2-3 clocks
      PORTA.OUTCLR = PIN4_bm; // Write PA4 Low - Atomic operation taking 2-3 words, 2-3 clocks
      break;

    case 11:
      PORTA.DIRCLR = PIN4_bm | PIN5_bm;    // Set A4, A5 input
      PORTA.OUTSET = PIN4_bp | PIN5_bp; // Write PA4 PA5 low, high impedance tri-state

      PORTB.DIR = PIN1_bm | PIN2_bm;    // Set B1 B2 output

      PORTB.OUTSET = PIN1_bm; // Write PB1 High - Atomic operation taking 2-3 words, 2-3 clocks
      PORTB.OUTCLR = PIN2_bm; // Write PB2 Low - Atomic operation taking 2-3 words, 2-3 clocks
      break;

    case 12:
      PORTB.DIRCLR = PIN1_bm | PIN2_bm;    // Set B1, B2 input
      PORTB.OUTSET = PIN1_bp | PIN2_bp; // Write PB1 PB2 low, high impedance tri-state

      PORTA.DIR = PIN4_bm | PIN5_bm;              // Set A4 A5 output

      PORTA.OUTSET = PIN5_bm; // Write PA5 High - Atomic operation taking 2-3 words, 2-3 clocks
      PORTA.OUTCLR = PIN4_bm; // Write PA4 Low - Atomic operation taking 2-3 words, 2-3 clocks
      break;
  }
}

void clearLEDs() {
  PORTC.DIRCLR = PIN0_bm | PIN1_bm | PIN2_bm;  // Set C0, C1, C2 input
  PORTC.OUTSET = PIN0_bp | PIN1_bp | PIN2_bp;// Write PC0, PC1, PC2 low, high impedance tri-state
  
  PORTB.DIRCLR = PIN0_bm | PIN1_bm | PIN2_bm;  // Set B0, B1, B2 input
  PORTB.OUTSET = PIN0_bp | PIN1_bp | PIN2_bp;// Write PB0, PB1, PB2 low, high impedance tri-state
  
  PORTA.DIRCLR = PIN4_bm | PIN5_bm;            // Set A4, A5 input
  PORTA.OUTSET = PIN4_bp | PIN5_bp;         // Write PA4, PA5 low, high impedance tri-state
}
