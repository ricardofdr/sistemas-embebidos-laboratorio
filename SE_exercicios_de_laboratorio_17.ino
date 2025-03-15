volatile int freemem;

void setup() {
  Serial.begin(9600);
  DDRD = 0 << DDD2;       // configure PD2/D2 as an input
  PORTD = 1 << PIND2;     // set a ‘1’ to an input PIN PD2/D2 to configure it as a pullup
  DDRB = 1 << DDB5;       // PB5/D13 is an output
  PCICR = 1 << PCIE2;     // enable pin change interrupt in PORTD (bank 2)
  PCMSK2 = 1 << PCINT18;  // enable pin change interrupt on pin PD2/D2 (PCINT18)
  asm("SEI");             //enable global interrupt
}

// function to return the amount of free RAM
int memoryFree() {
  extern int __bss_end;  // variables created by the build process when compiling the sketch
  extern void *__brkval;
  int freeValue;

  if ((int)__brkval == 0)
    freeValue = ((int)&freeValue) - ((int)&__bss_end);
  else
    freeValue = ((int)&freeValue) - ((int)__brkval);

  return freeValue;
}


void loop() {
  freemem = memoryFree();
  delay(2000);
  Serial.print("\n Memory available=");
  Serial.println(freemem);  // print the free memory
}

ISR(PCINT2_vect) {  // include the name of the interrupt vector (Pin change interrupt request 2)
  freemem = memoryFree();
  if (PIND & 1 << PIND2) {  // Check if PIN PD2/D2 in register PIND is ON (position can be checked using (1<<PIND2))
    PORTB = 0 << PORTB;     // turn the LED of pin PB5/D13 OFF
  } else {
    PORTB = 1 << PORTB5;  // turn the LED of pin PB5/D13 ON
  }
}
