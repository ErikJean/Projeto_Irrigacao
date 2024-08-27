#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define PINO_SENSOR 15
#define RELE_BOMBA 2
#define RELE_VALVULA 4

BluetoothSerial SerialBT;

char leitura;

unsigned long contador = 0;

unsigned long tempo;
unsigned long tempoEscolhido;

float fluxo;
float volume;
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
  leitura = '.';
  tempo = 0;
  tempoEscolhido = 0;
  volumeTotal = 0;
  SerialBT.print("\n--------------------------\n");
  SerialBT.print("\n Temporizador (");
  SerialBT.print(tempoEscolhido);
  SerialBT.print(")\n");
  SerialBT.print("\n 1 - 1 Segundo");
  SerialBT.print("\n 2 - 10 Segundos");
  SerialBT.print("\n 3 - 1 Minuto");
  SerialBT.print("\n 4 - 10 Minutos");
  SerialBT.print("\n 5 - Iniciar Temporizador");
  SerialBT.print("\n 0 - Limpar Temporizador\n");
  SerialBT.print("\n Escolha uma opção:\n");
  SerialBT.print("\n--------------------------\n");

  while (leitura != '1' && leitura != '2' && leitura != '3' && leitura != '4' && leitura != '5' && leitura != '0') {
    if (SerialBT.available() > 0) {
      leitura = SerialBT.read();
    }
  }

  if (leitura == '1') {
    tempoEscolhido += 1;
  }


  else if (leitura == '2') {
    tempoEscolhido += 10;
  }


  else if (leitura == '3') {
    tempoEscolhido += 60;
  }


  else if (leitura == '4') {
    tempoEscolhido += 600;
  }


  else if (leitura == '5') {
    SerialBT.print("\n---------------------------\n");
    SerialBT.print("\n Iniciando temporizador...\n");
    SerialBT.print("\n---------------------------\n");
  }


  else if (leitura == '0') {
    tempoEscolhido = 0;
  }

  if (leitura == '5') {
    while (tempo < tempoEscolhido) {
      contador = 0;

      attachInterrupt(digitalPinToInterrupt(PINO_SENSOR), contadorPulso, RISING);
      digitalWrite(RELE_VALVULA, HIGH);
      digitalWrite(RELE_BOMBA, HIGH);
      delay(1000);
      detachInterrupt(digitalPinToInterrupt(PINO_SENSOR));

      fluxo = (contador / 5.5) * 1000;  // Segundo a fórmula em L/min, multiplica por 1000 para encontrar o valor em ml
      volume = fluxo / 60;              // Divide por 60 para encontrar o volume medido a cada segundo
      volumeTotal += volume;            // Incrementa o valor de volume ao volume total

      SerialBT.printf("\nFluxo de: %f L/min", fluxo);
      SerialBT.printf("\nVolume: %f mL\n", volumeTotal);
      tempo++;
    }

    digitalWrite(RELE_VALVULA, LOW);
    digitalWrite(RELE_BOMBA, LOW);
  }
}

void contadorPulso() {
  contador++;
}
