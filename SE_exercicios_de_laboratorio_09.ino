void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  noInterrupts(); // desligar as interrupções     
}

void loop() {
  byte val = digitalRead(2);
  
  if(val){
    digitalWrite(13, LOW);
    // PORTD = 0<<PORTB5;
  }else{
    digitalWrite(13, HIGH);
    // PORTD = 1<<PORTB5;
  }
}

// VERSÃO ALTERNATIVA - INSTRUÇÕES DE MANIPULAÇÃO DOS REGISTOS
// void loop() {
//   PORTB = 1<<PORTB5;   // set the LED on
//   PORTB = 0<<PORTB5;    // set the LED off
// }
