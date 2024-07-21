#define PINO_SENSOR 15

// Para armazenar um tempo refente a execução do arduino
unsigned long tempoGravado = 0;

//definicao da variavel de contagem de voltas
unsigned long contador = 0;

// Variáveis para controle de dados
String mensagem;     // Controle de dados
char caractere;      // Controle de dados
bool irrigando = 0;  // Irrigação desativada ou ativada

float volumeAPI = 0;

// Definicao das variáveis de fluxo e volume
float fluxo = 0;
float volume = 0;
float volumeIrrigado = 0;

// Definicao do fator de calibracao para conversao do valor lido
float fatorCalibracao = 4.7;


void setup() {
  Serial.begin(9600);
  pinMode(PINO_SENSOR, INPUT_PULLUP);
}

void loop() {
  if (Serial.available()) {  // Leitura da comunicação
    caractere = Serial.read();

    if (caractere == '*') {
      volumeAPI = mensagem.toFloat();
      mensagem = "";
    }

    else if (caractere == ';') {
      fatorCalibracao = mensagem.toFloat();
      mensagem = "";
    }

    else if (caractere == '-') {
      irrigando = mensagem.toInt();
      mensagem = "";
    }

    else {
      mensagem.concat(caractere);
    }
  }

  if ((millis() - tempoGravado) > 1000) {  // Impressão no monitor serial
    Serial.print("<");
    Serial.print(irrigando);
    Serial.print(";");
    Serial.print(fatorCalibracao);
    Serial.print(" | ");
    Serial.print(volumeAPI);
    Serial.print(";");
    Serial.print(volumeIrrigado);
    Serial.println(">");

    tempoGravado = millis();
  }

  while (irrigando == 1) {
    sensorFluxo();

    if (Serial.available()) {  // Leitura da comunicação
      caractere = Serial.read();

      if (caractere == '*') {
        mensagem = "";
      }

      else if (caractere == ';') {
        mensagem = "";
      }

      else if (caractere == '-') {
        irrigando = mensagem.toInt();
        mensagem = "";

        if (irrigando == 0) {
          detachInterrupt(digitalPinToInterrupt(PINO_SENSOR));
          tempoGravado = 0;
        }
      }

      else {
        mensagem.concat(caractere);
      }
    }
  }
}

void sensorFluxo() {
  if ((millis() - tempoGravado) > 1000) {

    //desabilita a interrupcao para realizar a conversao do valor de pulsos
    detachInterrupt(digitalPinToInterrupt(PINO_SENSOR));

    //conversao do valor de pulsos para L/min
    fluxo = ((1000.0 / (millis() - tempoGravado)) * contador) / fatorCalibracao;

    //calculo do volume em L passado pelo sensor
    volume = fluxo / 60;

    //armazenamento do volume
    volumeIrrigado += volume;

    Serial.print("<");
    Serial.print(irrigando);
    Serial.print(";");
    Serial.print(fatorCalibracao);
    Serial.print(" | ");
    Serial.print(volumeAPI);
    Serial.print(";");
    Serial.print(volumeIrrigado);
    Serial.println(">");

    //reinicializacao do contador de pulsos
    contador = 0;

    //atualizacao da variavel tempo_antes
    tempoGravado = millis();

    //contagem de pulsos do sensor
    attachInterrupt(digitalPinToInterrupt(PINO_SENSOR), contadorPulso, FALLING);
  }
}

void contadorPulso() {
  contador++;
}
