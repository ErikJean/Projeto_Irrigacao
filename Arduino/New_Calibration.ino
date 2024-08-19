#include "BluetoothSerial.h"


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define PINO_SENSOR 15
#define RELE_BOMBA 2
#define RELE_VALVULA 4

BluetoothSerial SerialBT;

// Para armazenar um tempo refente a execução do Arduino
unsigned long tempoGravado = 0;

// Definicao da variável de contagem de voltas
unsigned long contador = 0;

// Controle de interface
unsigned long tempo;
long int tempo_escolhido = 0;
char leitura;

// Variável utilizada para exibir volume total de água
float volumeTotal;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("Controlador ESP32");
  Serial.println("\nO controlador ESP32 iniciou, você já pode parear com o bluetooth do seu smartphone!");
  pinMode(RELE_BOMBA, OUTPUT);
  pinMode(RELE_VALVULA, OUTPUT);
  digitalWrite(RELE_BOMBA, LOW);
  digitalWrite(RELE_VALVULA, LOW);
  pinMode(PINO_SENSOR, INPUT_PULLUP);
}

void loop() {
  if (SerialBT.connected()) {
    contador = 0;
    tempo_escolhido = 0;
    digitalWrite(RELE_BOMBA, LOW);
    digitalWrite(RELE_VALVULA, LOW);
    interface();

    if (leitura == '1') {
      temporizador();

      // Desabilita a interrupcão
      detachInterrupt(digitalPinToInterrupt(PINO_SENSOR));

      volumeTotal = ((5.5 * 1000) / 60) * contador;

      SerialBT.print("\n----------------------\n");
      SerialBT.print("\n Volume irrigado: ");
      SerialBT.println(volumeTotal);
      SerialBT.print(" Quantidade de pulsos total: ");
      SerialBT.println(contador);
      SerialBT.print("\n----------------------\n");
    }


    else if (leitura == '2') {
      cronometro();

      // Desabilita a interrupcão
      detachInterrupt(digitalPinToInterrupt(PINO_SENSOR));

      volumeTotal = ((5.5 * 1000) / 60) * contador;

      SerialBT.print("\n----------------------\n");
      SerialBT.print("\n Volume irrigado: ");
      SerialBT.println(volumeTotal);
      SerialBT.print(" Quantidade de pulsos total: ");
      SerialBT.println(contador);
      SerialBT.print("\n----------------------\n");
    }
  }
}

void interface() {
  leitura = '.';
  SerialBT.print("\n--------------------------\n");
  SerialBT.print("\n Software de Automação\n");
  SerialBT.print("\n 1 - Temporizador");
  SerialBT.print("\n 2 - Cronometro (Iniciar)");
  SerialBT.print("\n Escolha uma opção:\n");
  SerialBT.print("\n--------------------------\n");


  while (leitura != '1' && leitura != '2') {

    if (SerialBT.available() > 0) {
      leitura = SerialBT.read();
    }
  }
}

void temporizador() {
  while (leitura != '5' && leitura != '0') {
    leitura = '.';
    SerialBT.print("\n--------------------------\n");
    SerialBT.print("\n Temporizador (");
    SerialBT.print(tempo_escolhido / 3600000);
    SerialBT.print(":");
    SerialBT.print(((tempo_escolhido / 1000) / 60) % 60);
    SerialBT.print(":");
    SerialBT.print((tempo_escolhido / 1000) % 60);
    SerialBT.print(")\n");
    SerialBT.print("\n 1 - 1 Segundo");
    SerialBT.print("\n 2 - 10 Segundos");
    SerialBT.print("\n 3 - 1 Minuto");
    SerialBT.print("\n 4 - 10 Minutos");
    SerialBT.print("\n 5 - Iniciar Temporizador");
    SerialBT.print("\n 6 - Limpar Temporizador");
    SerialBT.print("\n 0 - SAIR\n");
    SerialBT.print("\n Escolha uma opção:\n");
    SerialBT.print("\n--------------------------\n");


    while (leitura != '1' && leitura != '2' && leitura != '3' && leitura != '4' && leitura != '5' && leitura != '6' && leitura != '0') {
      if (SerialBT.available() > 0) {
        leitura = SerialBT.read();
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
      SerialBT.print("\n---------------------------\n");
      SerialBT.print("\n Iniciando temporizador...\n");
      SerialBT.print("\n---------------------------\n");
    }


    else if (leitura == '6') {
      tempo_escolhido = 0;
    }
  }


  if (leitura == '5') {
    tempo = millis() + tempo_escolhido;

    while (millis() < tempo && leitura != '0') {
      digitalWrite(RELE_VALVULA, HIGH);
      digitalWrite(RELE_BOMBA, HIGH);
      sensorFluxo(false);
    }

    digitalWrite(RELE_BOMBA, LOW);
    digitalWrite(RELE_VALVULA, LOW);
  }
}

void cronometro() {
  tempo = millis();
  digitalWrite(RELE_VALVULA, HIGH);
  digitalWrite(RELE_BOMBA, HIGH);

  while (leitura != '0') {
    if (SerialBT.available() > 0) {
      leitura = SerialBT.read();
    }

    sensorFluxo(true);
  }


  if (leitura == '0') {
    digitalWrite(RELE_BOMBA, LOW);
    digitalWrite(RELE_VALVULA, LOW);
    tempo = millis() - tempo;
    SerialBT.print("\n----------------------------\n");
    SerialBT.print("\n Tempo cronometrado - ");
    SerialBT.print(tempo / 3600000);
    SerialBT.print(":");
    SerialBT.print(((tempo / 1000) / 60) % 60);
    SerialBT.print(":");
    SerialBT.println((tempo / 1000) % 60);
    SerialBT.print("\n----------------------------\n");
  }
}

void sensorFluxo(bool temp_cron) {
  // Contagem de pulsos do sensor
  attachInterrupt(digitalPinToInterrupt(PINO_SENSOR), contadorPulso, FALLING);

  if (SerialBT.available() > 0) {
    leitura = SerialBT.read();
  }

  if ((millis() - tempoGravado) >= 1000) {
    SerialBT.print("\n----------------------\n");
    SerialBT.print("\n Contagem de pulsos: ");
    SerialBT.println(contador);
    SerialBT.print("\n ");

    if (temp_cron) {
      SerialBT.print((millis() - tempo) / 3600000);
      SerialBT.print(":");
      SerialBT.print((((millis() - tempo) / 1000) / 60) % 60);
      SerialBT.print(":");
      SerialBT.print(((millis() - tempo) / 1000) % 60);
    }

    else {
      SerialBT.print((tempo - millis()) / 3600000);
      SerialBT.print(":");
      SerialBT.print((((tempo - millis()) / 1000) / 60) % 60);
      SerialBT.print(":");
      SerialBT.print(((tempo - millis()) / 1000) % 60);
    }

    SerialBT.println(" (0 - PARAR)");
    SerialBT.print("\n----------------------\n");

    tempoGravado = millis();
  }
}

void contadorPulso() {
  contador++;
}