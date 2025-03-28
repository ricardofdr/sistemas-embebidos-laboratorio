const int sensorPin = 0; // connect sensor to analog input 0
volatile long int rate=1023;

void setup()
{
	Serial.begin(9600);
  DDRB = 1<< DDB5; // PB5/D13 is an output
  ADMUX = 0<<REFS1 | 1<<REFS0 | 0<<ADLAR |0<<MUX3 | 0<<MUX2 | 0<<MUX1 | 0<<MUX0;; // Voltage reference for ADC=AVCC (5.0V); configurar ADLAR para right adjust for 10 bit resolution, select ADC0 (pino A0),
  ADCSRA = 1<<ADEN | 1<<ADSC | 1<<ADATE | 1<<ADIE | 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0;  // Enable ADC (ADEN), Start ADC conversion (ADSC) , Enable Auto Trigger (se for simulação fazer Disable!), enable ADC Interrupts,  configure ADPS2-0 for 128 prescale ( resulta numa frequência 16MHz/128=125kHz)
  ADCSRB |= 0 << ACME; // free running mode, (leave ACME as 0).
  DIDR0=0x3F; // desliga digital inputs nos pinos ADC0-ADC5 para reduzir consumo
  asm("SEI"); //enable global interrupt
}

void loop()
{
  // rate = analogRead(sensorPin); // read the analog input
  Serial.print("\nAnalog reading = ");
  Serial.print(rate);     // the raw analog reading
  PORTB = 1<< PORTB5; // set the LED on
  delay(rate); // wait duration dependent on light level
  PORTB = 0<< PORTB5; // set the LED off
  delay(rate);
}

ISR(ADC_vect)  // include the name of the interrupt vector (ADC) – ver tabela de interrupções nos slides da aula prática 3.
{
    rate = ADC;          // in this case 10 bits are being used but only needs to read the high value for 8 bit precision
    // REMEMBER: once ADCH is read the ADC will update
    ADCSRA|=1<<ADSC; // Se for simulação descomentar esta linha!
}
