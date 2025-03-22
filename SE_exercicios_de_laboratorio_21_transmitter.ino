/*
* SPI Trasmitter
* 
*/

#define BAUD_RATE 9600
#define BAUD_RATE_DIVISOR (F_CPU / 16 / BAUD_RATE - 1)

volatile unsigned char update=0; // flags an update to the blink rate of the slave
volatile char c;

void setup()
{
  USART_init();
  SPI_MasterInit(); 
  asm("SEI"); //enable global interrupt
  PORTB = 1 << PORTB2;   // set SS (pin PB2) High (inactive)
}

void loop()
{
  if (update==1){
    SPI_MasterTransmit(c);
    update=0; // update finalized
  }
}

//**************************************************************
//USART_init: initializes the USART system
//**************************************************************
void USART_init(void)
{
  UCSR0A = 0<<TXC0 | 0<<U2X0 | 0<<MPCM0;
  UCSR0B = 1<<RXCIE0 | 0<<TXCIE0 | 0<<UDRIE0 | 1<<RXEN0 | 1<<TXEN0 | 0<<UCSZ02 | 0<<TXB80;
  UCSR0C = 0<<UMSEL01 | 0<<UMSEL00 | 0<<UPM01 | 0<<UPM00 | 0<<USBS0 | 1<<UCSZ01 | 1<<UCSZ00 | 0<<UCPOL0;

  //Baud Rate initialization
  UBRR0 = BAUD_RATE_DIVISOR; 
}

ISR(USART_RX_vect) {
  c = UDR0; // read incoming byte to clear interrupt flag 
  update=1; // new character received. Flag an update transmission to the slave
}

//****************************************************
//SPI_MasterInit: initializes spi system
//****************************************************
void SPI_MasterInit(void)
{
  DDRB = (1 << DDB3) | (1 << DDB5) | (1 << DDB2) | (0 << DDB4); /* Set MOSI, SCK and SS as output. (MISO will be input) */
  SPCR = (0<<SPIE)|(1<<SPE)|(0<<DORD)|(1<<MSTR)|(0<<CPOL)|(0<<CPHA)|(0<<SPR1) |(1<<SPR0); // SPI interrupt disabled,  Enable SPI, DORD set to 0, set as Master, CPOL set to 0, CPHA set to 0, SPI Clock Rate = system clock/16
}

//****************************************************
//spi_write: Used by SPI master to transmit a data byte
//****************************************************
void SPI_MasterTransmit(char tx_byte)
{
  PORTB = 0 << PORTB2;             // set SS (Slave Select) LOW (active)
  SPDR = tx_byte;
  while (!(SPSR & (1<<SPIF)));   /* Wait for transmission complete  (Stay in the ‘while’ loop until flag ‘SPIF’ of register ‘SPSR’ becomes ’1’). */
    PORTB = 1 << PORTB2;             // set SS (Slave Select) High (inactive)
}

