#include <avr/sleep.h> 

volatile byte sec_ct = 0;
volatile byte min_ct = 0;
volatile byte hr_ct = 1;

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
  PORTA.DIRCLR = PIN1_bm;
  PORTA.PIN1CTRL = 0b00000011;

  set_sleep_mode(SLEEP_MODE_STANDBY);  // Set sleep mode to STANDBY mode
  sleep_enable();
}

void loop() {
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
  PORTA.INTFLAGS = PIN1_bm; // Clear flag

  
}
