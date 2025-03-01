/*
* SerialOutput sketch
* Print numbers to the serial port
*/
#define BAUD_RATE 9600
#define BAUD_RATE_DIVISOR (F_CPU / 16 / BAUD_RATE - 1)



// **************************************************************
// USART_init: initializes the USART system
// **************************************************************
void USART_init(void)
{
  UCSR0A = 0<<TXC0 | 0<<U2X0 | 0<<MPCM0; // Configure USART for: no Double Speed, Multi-processor Communication Mode disabled
  UCSR0B = 0<<RXCIE0 | 0<<TXCIE0 | 0<<UDRIE0 | 0<<RXEN0 | 1<<TXEN0 | 0<<UCSZ02 | 1<<TXB80;  // Configure USART for: interrupts and receiver disabled, transmitter enabled, 8 bit character size
  UCSR0C = 0<<UMSEL01 | 0<<UMSEL00 | 0<<UPM01 | 0<<UPM00 | 0<<USBS0 | 1<<UCSZ01 | 1<<UCSZ00 | 0<<UCPOL0;  // Configure USART for: Asynchronous operation, no parity, 1 stop bit, 8 bit character size

  //Baud Rate initialization
  UBRR0 = BAUD_RATE_DIVISOR; 
}

void setup()
{
  USART_init(); // send and receive at 9600 baud (default configuration
	                    // is 8 data bits, no parity, one stop bit)
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



int number = 0;
void loop()
{
  // Serial.print("\n The number is ");
  // Serial.println(number); // print the number
  usart_printstring("\n The number is ");
  usart_putinteger(number);

  delay(500); // delay half second between numbers
  number++; // to the next number
}
