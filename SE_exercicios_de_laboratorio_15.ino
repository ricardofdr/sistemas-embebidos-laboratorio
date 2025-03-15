/*
* SerialReceive sketch
* Blink the LED at a rate proportional to the received digit value
*/
#define BAUD_RATE 9600
#define BAUD_RATE_DIVISOR (F_CPU / 16 / BAUD_RATE - 1)
// int blinkRate=0; // blink rate stored in this variable
volatile int blinkRate=0;


// **************************************************************
// USART_init: initializes the USART system
// **************************************************************
void USART_init(void)
{
  UCSR0A = 0<<TXC0 | 0<<U2X0 | 0<<MPCM0; // Configure USART for: no Double Speed, Multi-processor Communication Mode disabled
  UCSR0B = 1<<RXCIE0 | 0<<TXCIE0 | 0<<UDRIE0 | 1<<RXEN0 | 1<<TXEN0 | 0<<UCSZ02 | 1<<TXB80;  // Configure USART for: interrupts and receiver disabled, transmitter enabled, 8 bit character size
  UCSR0C = 0<<UMSEL01 | 0<<UMSEL00 | 0<<UPM01 | 0<<UPM00 | 0<<USBS0 | 1<<UCSZ01 | 1<<UCSZ00 | 0<<UCPOL0;  // Configure USART for: Asynchronous operation, no parity, 1 stop bit, 8 bit character size

  //Baud Rate initialization
  UBRR0 = BAUD_RATE_DIVISOR; 
}



void setup()
{
  // Serial.begin(9600); // Initialize serial port to send and receive at 9600 baud
  USART_init();
  DDRB = 1<<PORTB5; // PB5/D13 is an output
  asm("SEI"); //enable global interrupt (alternativa: interrupts())
}



// **************************************************************
// USART_transmit: transmits  single byte of data
// **************************************************************
void USART_transmit(char data)
{
  while((UCSR0A & 1<<UDRE0) == 0); //wait for UDRE flag (Stay in the ‘while’ loop until flag ‘UDRE0’ of register ‘UCSR0A’ becomes ’1’. Suggestion: Position of the flag can be checked using ‘(1<<UDRE0)’ )
  UDR0 = data; //load data to UDR for transmission
}

//**************************************************************
//USART_receive: receives single byte of data
//**************************************************************
char USART_receive(void)
{
  while(!(UCSR0A & (1<<RXC0))); //wait for RXC flag (Stay in the ‘while’ loop until flag ‘RXC0’ of register ‘UCSR0A’ becomes ’1’. Suggestion: Position of the flag can be checked using ‘(1<< RXC0)’ )

  unsigned data = UDR0; //retrieve data from UDR
  return data;
}
//**************************************************************


ISR(USART_RX_vect) {
  // char c = UDR0; //VERSÃO ALTERNATIVA 
  char c = USART_receive(); // read incoming byte to clear interrupt flag (ATmega328P datasheet, section 20.11)

  /* Colocar aqui o processamento do byte recebido e actualização do blinkRate */
  // while (Serial.available()==0); // wait until at least one character is available

  // char ch = Serial.read(); // read the character
  if( isDigit(c) ) // is this an ascii digit between 0 and 9?
  {
    blinkRate = (c - '0'); // ASCII value converted to numeric value
    // Serial.print("Digit pressed: ");
    usart_printstring("\nDigit pressed:");
    // Serial.println(blinkRate); // print the number
    usart_putinteger(blinkRate);
    blinkRate = blinkRate * 100; // actual rate is 100ms times received digit
  }

  //Blink 5 times
  for (int j=0;j<5;j++){
    blink();
  }
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



void loop()
{
  PINB=1<<PORTB5; // Toggle pin PB5/D13 (0->1 and 1->0)   
  delay(blinkRate); // delay depends on blink rate value 
}

// blink the LED with the on and off times determined by blinkRate
void blink()
{
  PORTB = 1<<PORTB5; // PB5/D13 is an output
  delay(blinkRate); // delay depends on blink rate value
  PORTB = 0<<PORTB5; // PB5/D13 is an output
  delay(blinkRate);
}
