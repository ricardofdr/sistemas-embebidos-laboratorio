/*
* Brushed_H_Bridge sketch
* commands from serial port control motor speed and direction
* digits '0' through '9' are valid where '0' is off, '9' is max speed
* + or - set the direction
*/

void setup() {
  Serial.begin(9600);

  // Inicializar aqui os pinos 4, 5 e 7 (PD4, PD5 e PD7) como OUTPUT
  DDRD = 1 << DDD4 | 1 << DDD5 | 1 << DDD7;

  Serial.println("Speed (0-9) or + - to set direction");
}

void loop() {
  if (Serial.available()) {
    char ch = Serial.read();
    if (isDigit(ch))  // is ch a number?
    {
      int speed = map(ch, '0', '9', 0, 255);
      Serial.println(speed);

      analogWrite(5, speed);  //aplicar onda PWM no pino PD5 que controla o Enable da H-bridge.
    } else if (ch == '+') {
      Serial.println("CW");
      PORTD = 1 << PORTD4;  // Colocar pino PD4 (PORTD4) a HIGH sem estragar os outros! (Sugestão: Usar ‘|’)
      // PORTD &= 0 << PORTD7;  // Colocar pino PD7 (PORTD7) a LOW sem estragar os outros! (Sugestão: Usar ‘&’)
    } else if (ch == '-') {
      Serial.println("CCW");
      PORTD = 1 << PORTD7;  // Colocar pino PD7 (PORTD7) a HIGH sem estragar os outros! (Sugestão: Usar ‘|’)
      // PORTD &= 0 << PORTB4;  // Colocar pino PD4 (PORTD4) a LOW sem estragar os outros! (Sugestão: Usar ‘&’)
    } else {
      Serial.print("Unexpected character ");
      Serial.println(ch);
    }
  }
}
