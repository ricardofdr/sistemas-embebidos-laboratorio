const unsigned int precision = (1000000 / (F_CPU / 1000)) * 8;  // constant for conversion of counted delay to distance in cm
const unsigned int ns2cm = 58000;
volatile unsigned int start = 0;
volatile unsigned long atraso = 0;
volatile byte direccao = 1;  // guarda a direcção da contagem (1 é para cima)

void setup() {
  Serial.begin(9600);                // Open serial monitor at 9600 baud to see ping results.
  DDRB = (1 << DDB2) | (0 << DDB0);  // Inicializar pino 10 (PB2) como saída (trigger) e pino 8 (PB0) como entrada (echo)

  // initialize o modo 9 - PWM, Phase and Frequency Correct configurando WGM10, WGM11, WGM12 e WGM13 nos dois registos que se seguem (TCCR1A e TCCR1B)
  TCCR1A = 0 << COM1A1 | 0 << COM1A0 | 1 << COM1B1 | 0 << COM1B0 | 0 << WGM11 | 1 << WGM10;  // OC1A OFF, OC1B ON. Configurar WGM11, WGM10

  TCCR1B = 1 << WGM13 | 0 << WGM12 | 0 << CS12 | 1 << CS11 | 0 << CS10 | 1 << ICES1;  // configurar CS10, CS11, CS12 de forma a ter timer clock = F_CPU/8, configurar WGM12, WGM13
  TIMSK1 = 1 << ICIE1;                                                                // activar as interrupções do input capture interrupt (ICIE1) e do Output Compare A Match  (OCIE1A)
  OCR1A = 60000;                                                                      // pôr correspondente a ter 60000us de período (no modo 9 um período corresponde completar contagem crescente seguida de descrescente 0->OCR1A->0. Total de contagem num periodo = 2*OCR1A).
  OCR1B = 10;                                                                         // pôr correspondente a ter 10us de pulso de trigger (Nota: pulso é iniciado durante a contagem decrescente quando cruza o valor de OCR1B terminando quando é novamente atingido este valor na contagem crescente. Total de contagem com pulso ativo = 2*OCR1B)
  asm("SEI");
}

void loop() {
  delay(500);  // Wait 500ms between pings (about 2 pings/sec). 29ms should be the shortest delay between pings.
  Serial.print("Ping: ");
  Serial.print((atraso * precision) / ns2cm);  // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
  Serial.println("cm");
}

/* ISR interrupt vector */
ISR(TIMER1_CAPT_vect) {
  if (TCCR1B & (1 << ICES1))  // rising edge no Input Capture
  {
    start = ICR1;  // save the input capture value
    direccao = 1;  // o início do Echo pulse é sempre na contagem ascendente
  } else {         // Falling edge
    if (direccao == 1) {
      // ainda estava na fase ascendente da contagem
      atraso = ICR1 - start;
    } else {
      // já estava na fase descendente da contagem -> contabilizar a fase da subida
      atraso = 2 * OCR1A - ICR1 - start;
    }
  }
  TCCR1B ^= 1 << ICES1;  //Fazer o toggle do bit (0->1 ou 1->0) que desencadeia o Input Capture, ou seja o bit ICES1 (e apenas este bit!)
}

/* ISR interrupt vector */
ISR(TIMER1_COMPA_vect) {
  // Atingiu o topo da contagem. Vai iniciar a contagem decrescente
  direccao = 0;
}
