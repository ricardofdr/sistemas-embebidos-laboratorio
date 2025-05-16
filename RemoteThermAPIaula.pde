/**
 * GettingStarted
 *
 * A sketch to list the available serial ports
 * and display characters received
 */
import processing.serial.*;

Serial myPort; // Create object from Serial class

/* Communications and sensors related variables */
int portIndex = 1; // WARNING: Set this to the port connected to XBEE Explorer (0 is the first port)
byte frameStartByte = 0x7E;
byte frameTypeTXrequest = 0x10;
int destAddressHigh = 0x13A200;
//int destAddressLow = 0x40A099E8;
int destAddressLow = 0x4098229C;

int val; // Data received from the serial port
float temperature=-9;
int rssi=100;

/* drawing related variables */
int sizeX=35;
int sizeY=450;
int posX=115;
int posY=20;
int maxTemp = 40; // max of scale in degrees Celsius
int minTemp = -10; // min of scale in degrees Celsius
PFont font; // create a font for display
int fontSize = 12;
int rectX, rectY;      // Position of square button
int circleX, circleY;  // Position of circle button
int rectSize = 40;     // Diameter of rect button
int circleSize = 40;   // Diameter of circle button

void setup()
{
  size(800, 600); // screen size
  smooth(); // anti-aliasing for graphic display
  font = createFont("Arial.normal", fontSize);
  textFont(font);

  // Compute buttons location
  circleX = width/2+circleSize/2;
  circleY = height/2;
  rectX = width/2;
  rectY = height/2+rectSize+10;
  ellipseMode(CENTER);

  println(Serial.list()); // print the list of all the ports
  println(" Connecting to -> " + Serial.list()[portIndex]);
  myPort = new Serial(this, Serial.list()[portIndex], 9600);
  myPort.clear(); // clear buffer
}

void draw()
{
  background(224); // draw a light gray background

  if (myPort.available() >= 18) { // Wait until we have a mouthful of data

    val=decodeRXAPIpacket(val); // try to decode the received API frame from XBEE
    temperature=(val*3300.0/1024-500)/10; // 3300 mV of reference voltage and 10 bit resolution (1024 levels)

    println(temperature + " degrees C"); // Display message in Console
  }

  drawbuttons(); // Draw LED ON/OFF buttons
  therm_render(); // Draw thermometer
  drawTempdisplay();  // Draw Display box with temperatura value
//  drawRSSIdisplay();
}

void keyPressed() {
  // Called when a key button is pressed
  if (key == 'L' || key == 'l') {
    formatTXAPIpacket((byte) 'L'); // Turn on LED
  } 
  else if (key == 'D' || key == 'd') {
    formatTXAPIpacket((byte) 'D'); // Turn off LED
  }
}

void mousePressed() {
  // Called when a mouse button is pressed
  if ( overCircle(circleX, circleY, circleSize) ) {
    formatTXAPIpacket((byte) 'L'); // Turn on LED
  } 
  else if ( overRect(rectX, rectY, rectSize, rectSize) ) {
    formatTXAPIpacket((byte) 'D'); // Turn off LED
  }
}

boolean overRect(int x, int y, int width, int height) {
  // Detectar se o rato está sobre o botão rectangular
  if (mouseX >= x && mouseX <= x+width && 
    mouseY >= y && mouseY <= y+height) {
    return true;
  } 
  else {
    return false;
  }
}

boolean overCircle(int x, int y, int diameter) {
  // Detectar se o rato está sobre o botão circular
  float disX = x - mouseX;
  float disY = y - mouseY;
  if (sqrt(sq(disX) + sq(disY)) < diameter/2 ) {
    return true;
  } 
  else {
    return false;
  }
}

void drawbuttons() { 
  // draw buttons on screen
  fill(255);
  stroke(0);
  ellipse(circleX, circleY, circleSize, circleSize);   
  rect(rectX, rectY, rectSize, rectSize);  
  fill(0);
  text("Turn ON LED (or press 'L' or 'l')", circleX+circleSize+5, circleY); 
  text("Turn OFF LED (or press 'D' or 'd')", circleX+circleSize+5, rectY+rectSize/2);
}

void therm_render() { 
  // draw thermometer on screen
  noStroke(); // remove shape edges
  ellipseMode(CENTER); // center bulb
  float bulbSize = sizeX + (sizeX * 0.5); // determine bulb size
  int stemSize = 30; // stem augments fixed red bulb
  // to help separate it from moving mercury
  // limit display to range
  float displayTemp = round( temperature);
  if (temperature > maxTemp) {
    displayTemp = maxTemp + 1;
  }
  if ((int)temperature < minTemp) {
    displayTemp = minTemp;
  }
  // size for variable red area:
  float mercury = ( 1 - ( (displayTemp-minTemp) / (maxTemp-minTemp) ));
  // draw edges of objects in black
  fill(0);
  rect(posX-3, posY-3, sizeX+5, sizeY+5);
  ellipse(posX+sizeX/2, posY+sizeY+stemSize, bulbSize+4, bulbSize+4);
  rect(posX-3, posY+sizeY, sizeX+5, stemSize+5);
  // draw gray mercury background
  fill(64);
  rect(posX, posY, sizeX, sizeY);
  // draw red areas
  fill(255, 16, 16);
  // draw mercury area:
  rect(posX, posY+(sizeY * mercury), sizeX, sizeY-(sizeY * mercury));
  // draw stem area:
  rect(posX, posY+sizeY, sizeX, stemSize);
  // draw red bulb:
  ellipse(posX+sizeX/2, posY+sizeY + stemSize, bulbSize, bulbSize);
  // show text
  textAlign(LEFT);
  fill(0);
  textSize(10);
  // show maximum temperature:
  text(maxTemp + "°C", posX+sizeX + 5, posY);
  // show minimum temperature:
  text(minTemp + "°C", posX+sizeX + 5, posY + sizeY);
  // show temperature on thermometer:
  text(round(temperature) + " °C", posX+2, posY+(sizeY * mercury+ 14));
}


void drawTempdisplay() {
  // show temperature with higher numerical resolution inside a rectangle:
  fill(0);
  text("Temperature", width/2+15, posY+10);
  rect(width/2, posY+20, 120, 30, 7);
  fill(255);  // write text in white
  text(temperature + " °C", width/2+18, posY+40);
}

void formatTXAPIpacket(byte value) {
  // Transmit key pressed using XBEE API frame

    int sum = 0; // Accumulate the checksum  

  // API frame Start Delimiter
  myPort.write(frameStartByte);

  // Length - High and low parts of the frame length (Number of bytes between the length and the checksum)
  myPort.write((byte) 0x00);
  myPort.write((byte) 0x0F);

  // Frame Type - Indicate this frame contains a Transmit Request
  myPort.write(frameTypeTXrequest); 
  sum += frameTypeTXrequest;

  // Frame ID - set to zero for no reply
  myPort.write((byte) 0x00); 
  sum += 0x00;

  // 64-bit Destination Address - The following 8 bytes indicate the 64 bit address of the recipient (high and low parts).
  // Use 0xFFFF to broadcast to all nodes.
  myPort.write((byte) ((destAddressHigh >> 24) & 0xFF));
  myPort.write((byte) ((destAddressHigh >> 16) & 0xFF));
  myPort.write((byte) ((destAddressHigh >> 8) & 0xFF));
  myPort.write((byte) ((destAddressHigh) & 0xFF));
  sum += (byte) ((destAddressHigh >> 24) & 0xFF);
  sum += (byte) ((destAddressHigh >> 16) & 0xFF);
  sum += (byte) ((destAddressHigh >> 8) & 0xFF);
  sum += (byte) (destAddressHigh & 0xFF);
  myPort.write((byte) ((destAddressLow >> 24) & 0xFF));
  myPort.write((byte) ((destAddressLow >> 16) & 0xFF));
  myPort.write((byte) ((destAddressLow >> 8) & 0xFF));
  myPort.write((byte) ((destAddressLow & 0xFF)));
  sum += ((byte) (destAddressLow >> 24) & 0xFF);
  sum += ((byte) (destAddressLow >> 16) & 0xFF);
  sum += ((byte) (destAddressLow >> 8) & 0xFF);
  sum += ((byte) (destAddressLow & 0xFF));

  // 16-bit Destination Network Address - The following 2 bytes indicate the 16-bit address of the recipient.
  // Use 0xFFFE if the address is unknown.
  myPort.write((byte)0xFF);
  myPort.write((byte)0xFE);
  sum += 0xFF+0xFE;

  // Broadcast Radius - when set to 0, the broadcast radius will be set to the maximum hops value
  myPort.write((byte)0x00);
  sum += 0x00;

  // Options
  // 0x01 - Disable retries and route repair
  // 0x20 - Enable APS encryption (if EE=1)
  // 0x40 - Use the extended transmission timeout
  myPort.write((byte)0x00);
  sum += 0x00;

  // RF Data
  myPort.write(value);
  sum += value;

  // Checksum = 0xFF - the 8 bit sum of bytes from offset 3 (Frame Type) to this byte.
  myPort.write( (byte) (0xFF - ( sum & 0xFF)));
}

int decodeRXAPIpacket(int val) {
  // Function for decoding the received API frame from XBEE
  int rxvalue=0;

  while (myPort.read () != frameStartByte) {
    if (myPort.available()==0)
      return val; // No API frame present.
  }

  // Skip over the bytes in the API frame we don't care about (length, frame type, addresses, receive options)
  for (int i = 0; i < 14; i++) {
    myPort.read();
  }
  // The next two bytes represent the ADC measurement sent from the remote XBEE
  rxvalue = myPort.read() * 256; // add the most significant byte
  rxvalue += myPort.read(); // read the least significant byte

  // The next byte represents the rssi measurement sent from the remote XBEE
//  ????????

  myPort.read(); // Read  the last byte (Checksum) but don't store it

  return rxvalue;
}