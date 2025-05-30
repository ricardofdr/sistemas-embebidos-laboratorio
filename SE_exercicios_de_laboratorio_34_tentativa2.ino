const byte frameStartByte = 0x7E;
const byte frameTypeTXrequest = 0x10;
const byte frameTypeRXpacket = 0x90;
//const byte frameTypeATcommand = 0x08;
const byte frameTypeATresponse = 0x88;
const long destAddressHigh = 0x13A200;
// const long destAddressLow = 0x4098229C;
const long destAddressLow = 0x40A099E8;
char DBcommand[] = "DB";

int sensorPin = 0;   //the analog pin the TMP36's Vout (sense) pin is connected to
byte ATcounter = 0;  // for identifying current AT command frame
byte rssi = 0;       // RSSI value of last received packet

void setup() {
  DDRB = 1 << DDB5;           // PB5/D13 is an output
  Serial.begin(9600);         //Start the serial connection with the computer
  analogReference(EXTERNAL);  // Set the aref to 3.3v
}

void loop()  // run over and over again
{
  //getting the voltage reading from the temperature sensor
  int reading = analogRead(sensorPin);
  char key = 'nul';

  formatTXAPIpacket(reading);  // Transmit temperature reading

  if (Serial.available() >= 10) {  // Wait until we have a mouthful of data

    key = decodeAPIpacket();  // try to decode the received API frame from XBEE

    if (key == 'L') {
      PORTB = 1 << PORTB5;  // Turn ON LED
    } else if (key == 'D') {
      PORTB = 0 << PORTB5;  // Turn OFF LED
    }
  }

  delay(250);  //waiting 1 second
}

char decodeAPIpacket(void) {
  // Function for decoding the received API frame from XBEE
  char rxbyte = 'nul';
  byte frametype;

  while (Serial.read() != frameStartByte) {
    if (Serial.available() == 0)
      return rxbyte;  // No API frame present. Return 'nul'
  }

  // Skip over length field bytes in the API frame
  Serial.read();  // MSB
  Serial.read();  // LSB

  // Read received frame type
  frametype = Serial.read();

  if (frametype == frameTypeRXpacket) {
    // Zigbee Receive Packet API Frame
    while (Serial.available() < 13)
      ;  // the remainder of the Receive Packet API Frame should have 13 bytes, so only proceed when the full frame is in the Serial buffer

    // Skip over the bytes in the API frame we don't care about (addresses, receive options)
    for (int i = 0; i < 11; i++) {
      Serial.read();
    }
    // The next byte is the key pressed and sent from the remote XBEE
    rxbyte = Serial.read();

    Serial.read();             // Read  the last byte (Checksum) but don't store it
    formatATcommandAPI("DB");  // query the RSSI of the last received packet
  } else if (frametype == frameTypeATresponse) {
    // AT Command Response API frame
    while (Serial.available() < 6)
      ;

    for (int i = 0; i < 4; i++) {
      Serial.read();
    }
    rssi = Serial.read();
    Serial.read();
  }

  return rxbyte;
}

void formatTXAPIpacket(int value) {
  // Format and transmit a Transmit Request API frame

  long sum = 0;  // Accumulate the checksum

  // API frame Start Delimiter
  Serial.write(frameStartByte);

  // Length - High and low parts of the frame length (Number of bytes between the length and the checksum)
  Serial.write(0x00);
  Serial.write(0x11);

  // Frame Type - Indicate this frame contains a Transmit Request
  Serial.write(frameTypeTXrequest);
  sum += frameTypeTXrequest;

  // Frame ID - set to zero for no reply
  Serial.write(0x00);
  sum += 0x00;

  // 64-bit Destination Address - The following 8 bytes indicate the 64 bit address of the recipient (high and low parts).
  // Use 0xFFFF to broadcast to all nodes.
  Serial.write((destAddressHigh >> 24) & 0xFF);
  Serial.write((destAddressHigh >> 16) & 0xFF);
  Serial.write((destAddressHigh >> 8) & 0xFF);
  Serial.write((destAddressHigh)&0xFF);
  sum += ((destAddressHigh >> 24) & 0xFF);
  sum += ((destAddressHigh >> 16) & 0xFF);
  sum += ((destAddressHigh >> 8) & 0xFF);
  sum += (destAddressHigh & 0xFF);
  Serial.write((destAddressLow >> 24) & 0xFF);
  Serial.write((destAddressLow >> 16) & 0xFF);
  Serial.write((destAddressLow >> 8) & 0xFF);
  Serial.write(destAddressLow & 0xFF);
  sum += ((destAddressLow >> 24) & 0xFF);
  sum += ((destAddressLow >> 16) & 0xFF);
  sum += ((destAddressLow >> 8) & 0xFF);
  sum += (destAddressLow & 0xFF);

  // 16-bit Destination Network Address - The following 2 bytes indicate the 16-bit address of the recipient.
  // Use 0xFFFE if the address is unknown.
  Serial.write(0xFF);
  Serial.write(0xFE);
  sum += 0xFF + 0xFE;

  // Broadcast Radius - when set to 0, the broadcast radius will be set to the maximum hops value
  Serial.write(0x00);
  sum += 0x00;

  // Options
  // 0x01 - Disable retries and route repair
  // 0x20 - Enable APS encryption (if EE=1)
  // 0x40 - Use the extended transmission timeout
  Serial.write(0x00);
  sum += 0x00;

  // RF Data
  Serial.write((value >> 8) & 0xFF);  // ADC temperature reading (high byte)
  Serial.write(value & 0xFF);         // ADC temperature reading (low byte)
  Serial.write(rssi);  // RSSI reading
  sum += ((value >> 8) & 0xFF) + (value & 0xFF) + rssi;

  // Checksum = 0xFF - the 8 bit sum of bytes from offset 3 (Frame Type) to this byte.
  Serial.write(0xFF - (sum & 0xFF));

  delay(10);  // Pause to let the microcontroller settle down if needed
}

void formatATcommandAPI(char* command) {
  // Format and transmit a AT Command API frame
  long sum = 0;  // Accumulate the checksum

  ATcounter += 1;  // increment frame counter

  // API frame Start Delimiter
  Serial.write(frameStartByte);

  // Length - High and low parts of the frame length (Number of bytes between the length and the checksum)
  Serial.write(0x00);
  Serial.write(0x04);

  // Frame Type - Indicate this frame contains a AT Command
  Serial.write(0x08);
  sum += 0x08;

  // Frame ID – cannot be zero for receiving a reply
  Serial.write(ATcounter);
  sum += ATcounter;

  // AT Command
  Serial.write(command[0]);
  Serial.write(command[1]);
  sum += command[0];
  sum += command[1];

  // Parameter Value for the Command - Optional

  // Checksum = 0xFF - the 8 bit sum of bytes from offset 3 (Frame Type) to this byte.
  Serial.write(0xFF - (sum & 0xFF));

  delay(10);  // Pause to let the microcontroller settle down if needed
}
