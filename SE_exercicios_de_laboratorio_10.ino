void setup() {                
  DDRD = 0<<DDD2; // configure PD2/D2 as an input
  PORTD = 1<<PIND2;  // set a ‘1’ to an input PIN PD2/D2 to configure it as a pullup    
  DDRB = 1<<DDB5; // PB5/D13 is an output 
}

void loop() {
    if (PIND2) {    // define a correct condition involving register PIND
      PORTB = 0<<PORTB5;    // turn the LED of pin PB5/D13 OFF
    } 
    else {
      PORTB = 1<<PORTB5;    // turn the LED of pin PB5/D13 ON
    }
}
