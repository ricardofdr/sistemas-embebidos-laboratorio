void setup() {                
  asm("CLI"); // disable interrupts temporarily
  CLKPR = 1<<CLKPCE; // enable clock prescaler write sequence
  CLKPR = 1<<CLKPS3; // select clock divisor of 256
  // CLKPR = (1<<CLKPS2)|(1<<CLKPS1); // select clock divisor of 64
  asm("SEI"); // re-enable interrupts

  pinMode(13, OUTPUT);     // Substituir por DDRB = ??; // PB5/D13 is an output
}

void loop() {
  digitalWrite(13, HIGH);   // substituir por PORTB = ??; //set the LED on PB5 ON
  delay(1000);              // wait for a second
  digitalWrite(13, LOW);    // substituir por PORTB = ??; //set the LED on PB5 OFF
  delay(1000);              // wait for a second
}
