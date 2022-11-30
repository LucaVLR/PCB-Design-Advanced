#include <avr/sleep.h> 

volatile byte sec_ct = 0;
volatile byte min_ct = 0;
volatile byte hr_ct = 12;
volatile bool displayflag = false;

void setup() {
  // RTC setup
  RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;    // 32.768kHz Internal Crystal Oscillator (INT32K)
  while (RTC.STATUS > 0);               // Wait for all registers to be synchronized
  RTC.PER = 1000;                   // Set period for delay
  RTC.INTCTRL |= RTC_OVF_bm;            // Enable overflow Interrupt which will trigger ISR
  RTC.CTRLA = RTC_PRESCALER_DIV32_gc    // 32768 / 32 = 1024 (sec) ~ 1 ms
  | RTC_RTCEN_bm                        // Enable: enabled 
  | RTC_RUNSTDBY_bm;                    // Run In Standby: enabled

  // Button setup
  PORTA.DIR = PIN1_bp;  // Set PA1 input
  PORTA.PIN1CTRL = PORT_ISC_BOTHEDGES_gc; // enable interrupt on change (allows wake-up function)

  set_sleep_mode(SLEEP_MODE_STANDBY);  // Set sleep mode to STANDBY mode
  sleep_enable();
}

void loop() {
  if(displayflag) {  // if PA1 high
    writeMinutes(min_ct);
    writeHours(hr_ct);

    delay(1000);

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
      break;

    case 2:
      break;

    case 3:
      break;

    case 4:
      break;

    case 5:
      break;

    case 6:
      break;

    case 7:
      break;

    case 8:
      break;

    case 9:
      break;

    case 10:
      break;

    case 11:
      break;

    case 12:
      break;
  }
}

void clearLEDs() {
  PORTC.DIR = PIN0_bp | PIN1_bp | PIN2_bp;  // Set C0, C1, C2 input
  PORTC.OUTSET = PIN0_bp | PIN1_bp | PIN2_bp;// Write PC0, PC1, PC2 low, high impedance tri-state
  
  PORTB.DIR = PIN0_bp | PIN1_bp | PIN2_bp;  // Set B0, B1, B2 input
  PORTB.OUTSET = PIN0_bp | PIN1_bp | PIN2_bp;// Write PB0, PB1, PB2 low, high impedance tri-state
  
  PORTA.DIR = PIN4_bp | PIN5_bp;            // Set A4, A5 input
  PORTA.OUTSET = PIN4_bp | PIN5_bp;         // Write PA4, PA5 low, high impedance tri-state
}
