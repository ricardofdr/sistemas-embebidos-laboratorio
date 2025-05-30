#include <Servo.h>
Servo myservo;  // create servo object to control a servo
// int timeon = 1000;  // variable to store the servo position
int timeon;

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

  DDRB = (1 << DDB1);

  // initialize a seguir o modo “phase & frequency correct PWM” com fim da contagem (TOP) igual ao valor do registo ICR1 – Modo 8:

  TCCR1A = 1 << COM1A1 | 0 << COM1A0 | 1 << COM1B1 | 0 << COM1B0 | 0 << WGM11 | 0 << WGM10;
  // Configurar COM1A1, COM1A0 para o modo (“Clear OC1A/OC1B on Compare Match when upcounting. Set OC1A/OC1B on Compare Match when downcounting.”); Configurar COM1B1, COM1B0 para o mesmo modo ou para o modo normal/desligado; Configurar WGM11, WGM10

  TCCR1B = 1 << WGM13 | 0 << WGM12 | 0 << CS12 | 1 << CS11 | 0 << CS10;  // configurar CS10, CS11, CS12 de forma a ter timer clock = F_CPU/8 (=> 1us resolution para o modo 8); Configurar WGM12, WGM13

  ICR1 = 20000;  // periodo = 20ms
  OCR1A = 1500;  // ponto neutro = 1500us
}

// void loop() {
//   for (timeon = 1000; timeon < 2000; timeon += 5)  // goes from 0 degrees to 180 degrees
//   {                                                // in steps of 1 degree
//     myservo.writeMicroseconds(timeon);             // tell servo to go to position in variable ‘timeon’
//     delay(20);                                     // waits 20ms between servo commands
//   }
//   for (timeon = 2000; timeon >= 1000; timeon -= 5)  // goes from 180 degrees to 0 degrees
//   {
//     myservo.writeMicroseconds(timeon);  // move servo in opposite direction
//     delay(20);                          // waits 20ms between servo commands
//   }
// }

void loop() {
  timeon = analogRead(0);
  timeon = map(timeon, 0, 1023, 1000, 2000);
  // myservo.writeMicroseconds(timeon);
  OCR1A = timeon;
}
