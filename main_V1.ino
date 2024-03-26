#define RELE_BOMBA 7
#define RELE_VALVULA 8

//definicao do pino do sensor e de interrupcao
const int INTERRUPCAO_SENSOR = 0;  //interrupt = 0 equivale ao pino digital 2
const int PINO_SENSOR = 2;


//definicao da variavel de contagem de voltas
unsigned long contador = 0;


//definicao do fator de calibracao para conversao do valor lido
const float FATOR_CALIBRACAO = 1000.0;


//definicao das variaveis de fluxo e volume
float fluxo = 0;
float volume = 0;
float volume_total = 0;


//definicao da variavel de intervalo de tempo
unsigned long tempo_antes = 0;

unsigned long tempo;
char leitura;
long int tempo_escolhido = 0;

void setup() {
  Serial.begin(9600);
  pinMode(RELE_BOMBA, OUTPUT);
  pinMode(RELE_VALVULA, OUTPUT);
  digitalWrite(RELE_BOMBA, HIGH);
  digitalWrite(RELE_VALVULA, HIGH);
  pinMode(PINO_SENSOR, INPUT_PULLUP);
}

void loop() {
  tempo_escolhido = 0;
  digitalWrite(RELE_BOMBA, HIGH);
  digitalWrite(RELE_VALVULA, HIGH);
  interface();

  if (leitura == '1') {
    temporizador();
  }

  else if (leitura == '2') {
    cronometro();
  }
}

void interface() {
  leitura = '.';
  Serial.print("\n--------------------------\n");
  Serial.print("\n Software de Automação");
  Serial.print("\n 1 - Temporizador");
  Serial.print("\n 2 - Cronometro (Iniciar)\n");
  Serial.print("\n Escolha uma opção:\n");
  Serial.print("\n--------------------------\n");


  while (leitura != '1' && leitura != '2') {
    if (Serial.available() > 0) {
      leitura = Serial.read();
    }
  }
}

void temporizador() {
  while (leitura != '5' && leitura != '0') {
    leitura = '.';
    Serial.print("\n--------------------------\n");
    Serial.print("\n Temporizador (");
    Serial.print(tempo_escolhido / 3600000);
    Serial.print(":");
    Serial.print(((tempo_escolhido / 1000) / 60) % 60);
    Serial.print(":");
    Serial.print((tempo_escolhido / 1000) % 60);
    Serial.print(")\n");
    Serial.print("\n 1 - 1 Segundo");
    Serial.print("\n 2 - 10 Segundos");
    Serial.print("\n 3 - 1 Minuto");
    Serial.print("\n 4 - 10 Minutos");
    Serial.print("\n 5 - Iniciar Temporizador");
    Serial.print("\n 6 - Limpar Temporizador");
    Serial.print("\n 0 - SAIR\n");
    Serial.print("\n Escolha uma opção:\n");
    Serial.print("\n--------------------------\n");

    while (leitura != '1' && leitura != '2' && leitura != '3' && leitura != '4' && leitura != '5' && leitura != '6' && leitura != '0') {
      if (Serial.available() > 0) {
        leitura = Serial.read();
      }
    }

    if (leitura == '1') {
      tempo_escolhido += 1000;
    }

    else if (leitura == '2') {
      tempo_escolhido += 10000;
    }

    else if (leitura == '3') {
      tempo_escolhido += 60000;
    }

    else if (leitura == '4') {
      tempo_escolhido += 600000;
    }

    else if (leitura == '5') {
      Serial.print("\n---------------------------\n");
      Serial.print("\n Iniciando temporizador...\n");
      Serial.print("\n---------------------------\n");
    }

    else if (leitura == '6') {
      tempo_escolhido = 0;
    }
  }

  if (leitura == '5') {
    tempo = millis() + tempo_escolhido;
    volume_total = 0;

    while (millis() < tempo && leitura != '0') {
      digitalWrite(RELE_VALVULA, LOW);
      digitalWrite(RELE_BOMBA, LOW);
      sensor_fluxo(false);
    }

    digitalWrite(RELE_BOMBA, HIGH);
    digitalWrite(RELE_VALVULA, HIGH);
  }
}

void cronometro() {
  tempo = millis();
  digitalWrite(RELE_VALVULA, LOW);
  digitalWrite(RELE_BOMBA, LOW);
  volume_total = 0;

  while (leitura != '0') {
    if (Serial.available() > 0) {
      leitura = Serial.read();
    }
    sensor_fluxo(true);
  }

  if (leitura == '0') {
    digitalWrite(RELE_BOMBA, HIGH);
    digitalWrite(RELE_VALVULA, HIGH);
    tempo = millis() - tempo;
    Serial.print("\n----------------------------\n");
    Serial.print("\n Tempo cronometrado - ");
    Serial.print(tempo / 3600000);
    Serial.print(":");
    Serial.print(((tempo / 1000) / 60) % 60);
    Serial.print(":");
    Serial.println((tempo / 1000) % 60);
    Serial.print("\n----------------------------\n");
  }
}

void sensor_fluxo(bool temp_cron) {
  if ((millis() - tempo_antes) > 1000) {


    //desabilita a interrupcão para realizar a conversão do valor de pulsos
    detachInterrupt(INTERRUPCAO_SENSOR);


    if (Serial.available() > 0) {
      leitura = Serial.read();
    }


    //conversão do valor de pulsos para L/min
    fluxo = ((1000.0 / (millis() - tempo_antes)) * contador) / FATOR_CALIBRACAO;


    //exibicão do valor de fluxo
    Serial.print("\n----------------------\n");
    Serial.print("\n Fluxo de: ");
    Serial.print(fluxo);
    Serial.println(" L/min");


    //calculo do volume em L passado pelo sensor
    volume = fluxo / 60;


    //armazenamento do volume
    volume_total += volume;


    //exibicão do valor de volume
    Serial.print(" Volume: ");
    Serial.print(volume_total);
    Serial.println(" L\n");
    Serial.print(" ");


    if (temp_cron) {
      Serial.print((millis() - tempo) / 3600000);
      Serial.print(":");
      Serial.print((((millis() - tempo) / 1000) / 60) % 60);
      Serial.print(":");
      Serial.print(((millis() - tempo) / 1000) % 60);
    }

    else {
      Serial.print((tempo - millis()) / 3600000);
      Serial.print(":");
      Serial.print((((tempo - millis()) / 1000) / 60) % 60);
      Serial.print(":");
      Serial.print(((tempo - millis()) / 1000) % 60);
    }

    
    Serial.println(" (0 - PARAR)");
    Serial.print("\n----------------------\n");


    //reinicializacão do contador de pulsos
    contador = 0;


    //atualizacão da variável tempo_antes
    tempo_antes = millis();


    //contagem de pulsos do sensor
    attachInterrupt(INTERRUPCAO_SENSOR, contador_pulso, FALLING);
  }
}

void contador_pulso() {
  contador++;
}
