volatile byte pwm = 0;

void setup() {
  DDRB = 1 << DDB3;  // LED pin
  // TCCR2A = 0;                         // normal mode
  TCCR2A = 1 << COM2A1 | 1 << COM2A0 | 1 << WGM21 | 1 << WGM20;
  TCCR2B = 1 << CS22 | 1 << CS20;  // clock divider CK/128
  // TIMSK2 = 1 << OCIE2A | 1 << TOIE2;  // enable match and overflow interrupts
  TIMSK2 = 1 << TOIE2;
}
void loop() {
}

ISR(TIMER2_OVF_vect) {
  if (pwm) PORTB = 1 << PORTB3;  // LED on
  OCR2A = pwm;
  pwm++;
}

ISR(TIMER2_COMPA_vect) {
  if (pwm < 255) PORTB = 0 << PORTB3;  // LED off
}
