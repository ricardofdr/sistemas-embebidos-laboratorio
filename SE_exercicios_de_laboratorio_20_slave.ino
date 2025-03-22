/****************************************************************************/
/*
* I2C_Slave
 * Receive commands from master for blinking a LED
 */
#include <Wire.h>

const int my_address = 2; // the address to be used when operating as slave (choose between 1 and 127, must be different from other numbers already in use)
const int dest_address = 1; // address of the destination slave Arduino 
volatile int blinkRate=100; // blink rate stored in this variable

void setup()
{
  DDRD = 0 << DDD2; // PD2/D2 is an input
  PORTD = 1 << PORTD2;  // setting a ‘1’ to an input PIN configures it as a pullup

  DDRB = (1 << DDB5);  // Set PB5/D13 as an output (write a ‘1’ on the correct position of DDRB)
  Wire.begin(my_address); // join I2C bus using this address (insert correct address)
  Wire.onReceive(receiveEvent); // register event (I2C interrupt) to handle requests
  asm("SEI"); //enable global interrupt
}

void loop()
{
  unsigned char last_state=0xff;
  unsigned char pin_state = PIND & 1<<PIND2; // check PD2 pin value (position PIND2 on PIND register) – rever a forma usada no exercício 10

  PORTB = 1 << PORTB5; // Output High PB5/D13 (write a ‘1’ on the correct position of PORTB)
  delay(blinkRate); // delay depends on blink rate value
  PORTB = 0 << PORTB5; // Output Low PB5/D13 (write a ‘0’ on the correct position of PORTB)
  delay(blinkRate); // delay depends on blink rate value

  if (pin_state != last_state) // compare with last PD2 pin state
  {
	  last_state= pin_state; // store new PD2 pin state

    // transmit ‘last_state’ value to other device through I2C
    Wire.beginTransmission(dest_address); // transmit to other device (insert correct address)
 	  Wire.write(last_state); // Send new switch position (valor do last_state)
    Wire.endTransmission();
  }
}


void receiveEvent(int howMany)
{	
  while(Wire.available() > 0)
  {
    char c = Wire.read(); // receive byte as a character
    if( isDigit(c) ) // is this an ascii digit between 0 and 9?
    {
      blinkRate = (c - '0'); // ASCII value converted to numeric value
      blinkRate = blinkRate * 100; // actual rate is 100ms times received digit
    }
  }
}
