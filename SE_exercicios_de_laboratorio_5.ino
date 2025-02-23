void setup() {                
  DDRB = 1<<DDB5;
}

void loop() {
  PORTB = 1 << PORTB5;
  delay(1000);
  PORTB = 0 << PORTB5;
  delay(1000);
}

//VERSÃO ALTERNATIVA
// void loop() {
//   PINB = 1 << PINB5;
//   delay(1000);
// }
