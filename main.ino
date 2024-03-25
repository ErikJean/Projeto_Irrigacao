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

void sensor_fluxo() {
  if ((millis() - tempo_antes) > 1000) {


    //desabilita a interrupcao para realizar a conversao do valor de pulsos
    detachInterrupt(INTERRUPCAO_SENSOR);


    //conversao do valor de pulsos para L/min
    fluxo = ((1000.0 / (millis() - tempo_antes)) * contador) / FATOR_CALIBRACAO;


    //exibicao do valor de fluxo
    Serial.print("\nFluxo de: ");
    Serial.print(fluxo);
    Serial.println(" L/min");


    //calculo do volume em L passado pelo sensor
    volume = fluxo / 60;


    //armazenamento do volume
    volume_total += volume;


    //exibicao do valor de volume
    Serial.print("Volume: ");
    Serial.print(volume_total);
    Serial.println(" L");


    //reinicializacao do contador de pulsos
    contador = 0;


    //atualizacao da variavel tempo_antes
    tempo_antes = millis();


    //contagem de pulsos do sensor
    attachInterrupt(INTERRUPCAO_SENSOR, contador_pulso, FALLING);
  }
}

void contador_pulso() {
  contador++;
}
