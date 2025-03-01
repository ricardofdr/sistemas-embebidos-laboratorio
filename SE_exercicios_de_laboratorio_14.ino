/*
* SerialReceive sketch
* Blink the LED at a rate proportional to the received digit value
*/
int blinkRate=0; // blink rate stored in this variable

void setup()
{
  Serial.begin(9600); // Initialize serial port to send and receive at 9600 baud
  DDRB = 1<<PORTB5; // PB5/D13 is an output
}

// //**************************************************************
// //USART_receive: receives single byte of data
// //**************************************************************
// char USART_receive(void)
// {
//   while(????); //wait for RXC flag (Stay in the ‘while’ loop until flag ‘RXC0’ of register ‘UCSR0A’ becomes ’1’. Suggestion: Position of the flag can be checked using ‘(1<< RXC0)’ )

//   unsigned data = UDR0; //retrieve data from UDR
//   return data;
// }
// //**************************************************************


void loop()
{
  while (Serial.available()==0); // wait until at least one character is available

  char ch = Serial.read(); // read the character
  if( isDigit(ch) ) // is this an ascii digit between 0 and 9?
  {
    blinkRate = (ch - '0'); // ASCII value converted to numeric value
    Serial.print("Digit pressed: ");
    Serial.println(blinkRate); // print the number
    blinkRate = blinkRate * 100; // actual rate is 100ms times received digit
  }

  //Blink 5 times
  for (int j=0;j<5;j++){
    blink();
  }
}

// blink the LED with the on and off times determined by blinkRate
void blink()
{
  PORTB = 1<<PORTB5; // PB5/D13 is an output
  delay(blinkRate); // delay depends on blink rate value
  PORTB = 0<<PORTB5; // PB5/D13 is an output
  delay(blinkRate);
}
