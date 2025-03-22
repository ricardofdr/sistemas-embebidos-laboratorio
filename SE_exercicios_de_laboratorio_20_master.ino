/*
* I2C_Master
* Echo Serial data to an I2C slave
*/
#include <Wire.h>

#define BAUD_RATE 9600
#define BAUD_RATE_DIVISOR (F_CPU / 16 / BAUD_RATE - 1)

const int my_address = 1; // the address to be used when operating as slave (choose between 1 and 127, must be different from other numbers already in use)
const int dest_address = 2; // address of the destination slave Arduino 
volatile unsigned char update=0; // flags an update to the blink rate of the slave
volatile char c;

void setup()
{
  DDRB = 1 << DDB5;  // Set PB5/D13 is an output
  USART_init();
  Wire.begin(my_address); // join I2C bus using this address (insert correct address)
  Wire.onReceive(receiveEvent); // register event (I2C interrupt) to handle requests
  asm("SEI"); //enable global interrupt
}


void loop()
{
  if (update==1){
    Wire.beginTransmission(dest_address); // transmit to slave device (insert correct address)
    Wire.write(c);
    Wire.endTransmission();
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

void receiveEvent(int howMany)
{
  	while(Wire.available() > 0)
 	 {
    		unsigned char ledOFF= Wire.read(); // receive byte as a character

    		// turn ON/OFF LED on PB5/D13 depending on the character arriving on I2C
    		if (ledOFF){
     		    PORTB = 0 << PORTB5; // Set PB5/D13 to Low (write a ‘0’ on the correct position of PORTB)
    		} else {
      		   PORTB = 1 << PORTB5; // Set PB5/D13 to High (write a ‘1’ on the correct position of PORTB)
    		}
    	}
  }

