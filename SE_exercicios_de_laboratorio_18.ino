/*
Interrupts sketch
*/
#define BAUD_RATE 9600
#define BAUD_RATE_DIVISOR (F_CPU / 16 / BAUD_RATE - 1)

const int numberOfEntries = 64; // Limite da contagem
volatile unsigned long miliseconds;
volatile byte index = 0;
volatile unsigned long results[numberOfEntries];

void USART_init(void)
{
  UCSR0A = 0<<TXC0 | 0<<U2X0 | 0<<MPCM0; // Configure USART for: no Double Speed, Multi-processor Communication Mode disabled
  UCSR0B = 1<<RXCIE0 | 0<<TXCIE0 | 0<<UDRIE0 | 1<<RXEN0 | 1<<TXEN0 | 0<<UCSZ02 | 0<<TXB80;  // Configure USART for: interrupts and receiver disabled, transmitter enabled, 8 bit character size
  UCSR0C = 0<<UMSEL01 | 0<<UMSEL00 | 0<<UPM01 | 0<<UPM00 | 0<<USBS0 | 1<<UCSZ01 | 1<<UCSZ00 | 0<<UCPOL0;  // Configure USART for: Asynchronous operation, no parity, 1 stop bit, 8 bit character size

  //Baud Rate initialization
  UBRR0 = BAUD_RATE_DIVISOR; 
}

// **************************************************************
// USART_transmit: transmits  single byte of data
// **************************************************************
void USART_transmit(char data)
{
  while((UCSR0A & 1<<UDRE0) == 0); //wait for UDRE flag (Stay in the ‘while’ loop until flag ‘UDRE0’ of register ‘UCSR0A’ becomes ’1’. Suggestion: Position of the flag can be checked using ‘(1<<UDRE0)’ )
  UDR0 = data; //load data to UDR for transmission
}

void usart_printstring(char *s) {
  while(*s) {   // end of string indicated by  the null character (=0)
    USART_transmit(*s);
    s++;
  }
}
void usart_putinteger(long i) {
  char s[25]; // character buffer to build string in

  itoa(i, s, 10); // convert integer to ASCII string, base 10
  usart_printstring (s); // now print the string on the serial port
}

void setup()
{
  DDRD &= ~(1 << DDD2);// inicializar pino 2 (PD2)  do Arduino como INPUT  
  DDRB |= (1 << DDB5);// inicializar o pino 13 (PB5) como OUTPUT;
  USART_init();// inicializar USART (usar funções do exercício 13 ou da biblioteca Serial do Arduino)
  EICRA |= (1 << ISC01)|(1 << ISC00); // Enable interrupts on rising edge on INT0
  EIMSK |= (1 << INT0);// Enable Interrupt INT0
  asm("SEI"); //enable global interrupt
  results[0]=0;

  miliseconds = 0;
}

void loop()
{
	int delays=random(10);

	PORTB |= (1<<PORTB5); // Set PB5/D13 to High (write a ‘1’ on the correct position of PORTB)
  delay(delays); // delay depends on blink rate value
 	PORTB &= ~(1<<PORTB5); // Set PB5/D13 to Low (write a ‘0’ on the correct position of PORTB)
 	delay(delays); // delay depends on blink rate value

  if(index >= numberOfEntries)
  {
    usart_printstring("\nDurations in Miliseconds are:"); // Escrever pela USART a mensagem "Durations in Miliseconds are:" (usar funções do exercício 13 ou da biblioteca Serial do Arduino)

    for( byte i=0; i < numberOfEntries; i++)
    {
      usart_putinteger(results[i]);  // Escrever pela USART o valor de results[i] (usar funções do exercício 13 ou da biblioteca Serial do Arduino)
      usart_printstring(", ");
    }
    index = 0; // start analyzing again
  }
  delay(50);
}

ISR(INT0_vect)  // include the name of the interrupt vector (external interrupt request 0) – ver tabela de interrupções nos slides da aula prática 3.
{
  if(index < numberOfEntries )
  {
    if(index > 0)
    {
      results[index] = millis() - miliseconds;
      // USART_transmit(", ");
    }
    index = index + 1;
  }
  miliseconds = millis();
}
