#define OFFSET 347

void setup() {
  Serial.begin(9600);
  Serial.println("Arduino Uno as Thermometer");
  ADMUX = 1<<REFS1 | 1<<REFS0 | 1<<MUX3 | 0<<MUX2 | 0<<MUX1 | 0<<MUX0; // internal 1.1V reference e ADC channel "8"
  ADCSRA = 1<<ADEN | 1<<ADSC | 0<<ADIE | 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0; // enable ADC, start conversion, ADC Interrupt disabled, clock divisor=128 (16MHz/128=125 KHz clock)
}

void loop() {
  while(ADCSRA & (1 << ADSC));      // This line waits for the ADC to finish
  Serial.print(ADC - OFFSET);
  Serial.println("C");
  ADCSRA |= 1 << ADSC; // start next conversion (escrever um ‘1’ na posição ADSC do registo ADCSRA sem estragar as outras!)  Alternativa: bitSet(ADCSRA, ADSC);
  delay(1000); // wait
}
