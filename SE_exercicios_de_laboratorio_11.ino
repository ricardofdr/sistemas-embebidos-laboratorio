/*
  Blink
  Turns on a LED for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
/*
void setup() { 
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  //pinMode(13, OUTPUT);
  asm("CLI"); // disable interrupts temporarily
  CLKPR = 1<<CLKPCE; // enable clock prescaler write sequence
  CLKPR = (1<<CLKPS2) | (1<<CLKPS1) ; // select clock divisor of 256
  asm("SEI"); // re-enable interrupts
  //DDRB = 1<<DDB5; // PB5/D13 is an output
  pinMode(13, INPUT_PULLUP);
  noInterrupts();
}

void loop() {
  //digitalWrite(13, HIGH);   // set the LED on
  PORTB = 0<<PORTB5; // LED is on - alternative: PORTB = 0x20;
  delay(10); // wait for a second
  //digitalWrite(13, LOW);   // set the LED off
  PORTB = 1<<PORTB5; // LED is off - alternative: PORTB = 0x00;
  delay(10); // wait for a second
}*/

void setup() {                
  DDRB = 1<<DD; // PB5/D13 is an output
  PORTD = 1<<PD2;  // set a ‘1’ to an input PIN PD2/D2 to configure it as a pullup    
  DDRB = 1<<DDB5; // PB5/D13 is an output 
}

void loop() {
    if (???) {    // define a correct condition involving register PIND
      PORTB = 0<<PORTB5;    // turn the LED of pin PB5/D13 OFF
    } 
    else {
      PORTB = 1<<PORTB5;    // turn the LED of pin PB5/D13 ON
    }
}

