void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);
  noInterrupts(); // desligar as interrupções     
}

void loop() {
  digitalWrite(13, HIGH);   // set the LED on
  digitalWrite(13, LOW);    // set the LED off
}

// VERSÃO ALTERNATIVA - INSTRUÇÕES DE MANIPULAÇÃO DOS REGISTOS
// void loop() {
//   PORTB = 1<<PORTB5;   // set the LED on
//   PORTB = 0<<PORTB5;    // set the LED off
// }
