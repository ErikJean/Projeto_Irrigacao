#include "BluetoothSerial.h"


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


#define RELE_BOMBA 2
#define RELE_VALVULA 4


BluetoothSerial SerialBT;


//definicao do pino do sensor e de interrupcao
const int INTERRUPCAO_SENSOR = 15;  //interrupt = 0 equivale ao pino digital 2
const int PINO_SENSOR = 15;




//definicao da variavel de contagem de voltas
unsigned long contador = 0;




//definicao do fator de calibracao para conversao do valor lido
float FATOR_CALIBRACAO = 4.7;




//definicao das variaveis de fluxo e volume
float fluxo = 0;
float volume = 0;
float volume_total = 0;




//definicao da variavel de intervalo de tempo
unsigned long tempo_antes = 0;


unsigned long tempo;
char leitura;
long int tempo_escolhido = 0;
String str = "";


void setup() {
  Serial.begin(115200);
  SerialBT.begin("Controlador ESP32");
  Serial.println("\nO controlador ESP32 iniciou, você já pode parear com o bluetooth do seu smartphone!");
  pinMode(RELE_BOMBA, OUTPUT);
  pinMode(RELE_VALVULA, OUTPUT);
  digitalWrite(RELE_BOMBA, !HIGH);
  digitalWrite(RELE_VALVULA, !HIGH);
  pinMode(PINO_SENSOR, INPUT_PULLUP);
}


void loop() {
  noInterrupts();
  if (SerialBT.connected()) {
    tempo_escolhido = 0;
    digitalWrite(RELE_BOMBA, !HIGH);
    digitalWrite(RELE_VALVULA, !HIGH);
    interface();


    if (leitura == '1') {
      temporizador();
    }


    else if (leitura == '2') {
      cronometro();
    }

    else if (leitura == '3') {
      calibracao();
    }
  }
}

void interface() {
  leitura = '.';
  SerialBT.print("\n--------------------------\n");
  SerialBT.print("\n Software de Automação (Fator atual = ");
  SerialBT.print(FATOR_CALIBRACAO);
  SerialBT.print(")\n");
  SerialBT.print("\n 1 - Temporizador");
  SerialBT.print("\n 2 - Cronometro (Iniciar)");
  SerialBT.print("\n 3 - Fator de Calibração\n");
  SerialBT.print("\n Escolha uma opção:\n");
  SerialBT.print("\n--------------------------\n");


  while (leitura != '1' && leitura != '2' && leitura != '3') {

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
    volume_total = 0;


    while (millis() < tempo && leitura != '0') {
      digitalWrite(RELE_VALVULA, !LOW);
      digitalWrite(RELE_BOMBA, !LOW);
      sensor_fluxo(false);
    }


    digitalWrite(RELE_BOMBA, !HIGH);
    digitalWrite(RELE_VALVULA, !HIGH);
  }
}


void cronometro() {
  tempo = millis();
  digitalWrite(RELE_VALVULA, !LOW);
  digitalWrite(RELE_BOMBA, !LOW);
  volume_total = 0;


  while (leitura != '0') {
    if (Serial.available() > 0) {
      leitura = Serial.read();
    }


    if (SerialBT.available() > 0) {
      leitura = SerialBT.read();
    }
    sensor_fluxo(true);
  }


  if (leitura == '0') {
    digitalWrite(RELE_BOMBA, !HIGH);
    digitalWrite(RELE_VALVULA, !HIGH);
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


void calibracao() {
  str = "";

  while (true) {
    if (SerialBT.available() > 0) {
      str = SerialBT.readString();
      break;
    }
  }
  FATOR_CALIBRACAO = str.toFloat();
}

void sensor_fluxo(bool temp_cron) {
  if ((millis() - tempo_antes) > 1000) {




    //desabilita a interrupcão para realizar a conversão do valor de pulsos
    detachInterrupt(INTERRUPCAO_SENSOR);




    if (Serial.available() > 0) {
      leitura = Serial.read();
    }




    if (SerialBT.available() > 0) {
      leitura = SerialBT.read();
    }




    //conversão do valor de pulsos para L/min
    fluxo = ((1000.0 / (millis() - tempo_antes)) * contador) / FATOR_CALIBRACAO;




    //exibicão do valor de fluxo
    SerialBT.print("\n----------------------\n");
    SerialBT.print("\n Fluxo de: ");
    SerialBT.print(fluxo);
    SerialBT.println(" L/min");




    //calculo do volume em L passado pelo sensor
    volume = fluxo / 60;




    //armazenamento do volume
    volume_total += volume;




    //exibicão do valor de volume
    SerialBT.print(" Volume: ");
    SerialBT.print(volume_total*1000);
    SerialBT.println(" mL\n");
    SerialBT.print(" ");




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




    //reinicializacão do contador de pulsos
    contador = 0;




    //atualizacão da variável tempo_antes
    tempo_antes = millis();




    //contagem de pulsos do sensor
    attachInterrupt(digitalPinToInterrupt(INTERRUPCAO_SENSOR), contador_pulso, FALLING);
  }
}


void contador_pulso() {
  contador++;
}
