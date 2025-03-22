/*********************************************************************/
/*
* SPI Receiver
 */
volatile int blinkRate=100; // blink rate stored in this variable

void setup()
{
  DDRD = 1 << DDD2;  // Set PD2/D2 as an output (write a ‘1’ on the correct position of DDRD)
  SPI_SlaveInit(); // Initialize SPI
  asm("SEI"); //enable global interrupt
}

void loop()
{
  PORTD = 1 << PORTD2; // Set PD2/D2 to High (write a ‘1’ on the correct position of PORTD)
  delay(blinkRate); // delay depends on blink rate value
  PORTD = 0 << PORTD2; // Set PD2/D2 to Low (write a ‘0’ on the correct position of PORTD)
  delay(blinkRate); // delay depends on blink rate value  
}

//****************************************************
//SPI_SlaveInit: initializes spi system
//****************************************************
void SPI_SlaveInit (void)
{
  DDRB = 1 << DDB4;  /* Set MISO as output */
  SPCR = (1<<SPIE)|(1<<SPE)|(0<<DORD)|(0<<MSTR)|(0<<CPOL)|(0<<CPHA)|(0<<SPR1) |(1<<SPR0); // Enable SPI interrupt,  Enable SPI, DORD set to 0, set as Slave, CPOL set to 0,CPHA set to 0, SPI Clock Rate not required for slave
}

//****************************************************
//spi_read: Used by SPI slave to receive data byte (using interrupts)
//****************************************************
ISR(SPI_STC_vect)  // include the name of the interrupt vector (SPI Serial Transfer Complete) – ver tabela de interrupções nos slides da aula prática 3.
{
    char c = SPDR;
    
    if( isDigit(c) ) // is this an ascii digit between 0 and 9?
    {
      blinkRate = (c - '0'); // ASCII value converted to numeric value
      blinkRate = blinkRate * 100; // actual rate is 100ms times received digit
    }
}
