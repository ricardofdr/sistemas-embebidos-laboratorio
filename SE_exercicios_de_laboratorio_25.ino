
void setup() {
  DDRB = 1 << DDB5;

  // TCCR1A = 0 << COM1A1 | 0 << COM1A0 | 0 << COM1B1 | 0 << COM1B0 | 0 << WGM11 | 0 << WGM10;  // Normal port operation ( OC1A/OC1B disconnected); Normal mode (mode 0),
  // TCCR1B = 0 << WGM13 | 0 << WGM12 | 1 << CS12 | 0 << CS11 | 0 << CS10;                      // Normal mode (mode 0), divide clock by 256
  // TIMSK1 = 1 << TOIE1;                                                                       // enable Timer/Counter1 overflow interrupt

  // ALTERNATIVA
  // TCCR1A = 0 << COM1A1 | 0 << COM1A0 | 0 << COM1B1 | 0 << COM1B0 | 0 << WGM11 | 0 << WGM10;  // Normal port operation ( OC1A/OC1B disconnected); CTC mode with ICR1 (mode 12),
  // TCCR1B = 1 << WGM13 | 1 << WGM12 | 1 << CS12 | 0 << CS11 | 1 << CS10;                      // CTC mode with ICR1 (mode 12) and divide clock by 1024 (16,000,000 ÷ 1024 = 15,625Hz)
  // ICR1 = 15624;                                                                              // used as compare value for counter
  // TIMSK1 = 1 << ICIE1;                                                                       // enable input capture interrupt

  // ALTERNATIVA 2
  TCCR1A = 0 << COM1A1 | 0 << COM1A0 | 0 << COM1B1 | 0 << COM1B0 | 0 << WGM11 | 0 << WGM10;  // Normal port operation (OC1A/OC1B disconnected); CTC mode with OCR1A (mode 4),
  TCCR1B = 0 << WGM13 | 1 << WGM12 | 1 << CS12 | 0 << CS11 | 1 << CS10;                      // CTC mode with OCR1A (mode 4) and divide clock by 1024
  OCR1A = 15624;                                                                             // used as compare value for counter
  TIMSK1 = 1 << OCIE1A;                                                                      // enable Output Compare A Match interrupt


  asm("SEI");
}

void loop() {
}

// ISR(TIMER1_OVF_vect) {
//   PINB = 1 << PORTB5;
// }

// ISR(TIMER1_CAPT_vect) {
//   PINB = 1 << PORTB5;
// }

ISR(TIMER1_COMPA_vect) {
  PINB = 1 << PORTB5;
}
