#define RELE 7

unsigned long tempo_arduino;
char leitura;
long int tempo_escolhido = 0;

void setup() {
  Serial.begin(9600);
  pinMode(RELE, OUTPUT);
  digitalWrite(RELE, HIGH);
}

void loop() {
  tempo_escolhido = 0;
  interface();


  if (leitura == '1') {
    temporizador();
  }
}

void interface() {
  leitura = '.';
  Serial.print("\n-----------------------\n");
  Serial.print("\n Software de Automação");
  Serial.print("\n 1 - Temporizador");
  Serial.print("\n 2 - Cronometro\n");
  Serial.print("\n Escolha uma opção:\n");
  Serial.print("\n-----------------------\n");


  while (leitura != '1' && leitura != '2') {
    if (Serial.available() > 0) {
      leitura = Serial.read();
    }
  }
}

void temporizador() {
}