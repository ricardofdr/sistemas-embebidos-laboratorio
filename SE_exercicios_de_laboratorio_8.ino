void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);
  pinMode(2, INPUT);
  noInterrupts(); // desligar as interrupções     
}

void loop() {
  byte val = digitalRead(2);
  
  if(val){
    digitalWrite(13, HIGH);
  }else{
    digitalWrite(13, LOW);
  }
}

// VERSÃO ALTERNATIVA - INSTRUÇÕES DE MANIPULAÇÃO DOS REGISTOS
// void loop() {
//   PORTB = 1<<PORTB5;   // set the LED on
//   PORTB = 0<<PORTB5;    // set the LED off
// }