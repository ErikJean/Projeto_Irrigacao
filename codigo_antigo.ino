#define RELE 7
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
char info;
long int tempo_escolhido = 0;


void setup() {
  Serial.begin(9600);
  pinMode(RELE, OUTPUT);
  pinMode(RELE_VALVULA, OUTPUT);
  digitalWrite(RELE, HIGH);
  digitalWrite(RELE_VALVULA, HIGH);
  pinMode(PINO_SENSOR, INPUT_PULLUP);
}


void loop() {
  tempo_escolhido = 0;
  info = '.';
  Serial.println("\nTemporizador:\n\n1 - 1 Segundo\n2 - 10 Segundos\n3 - 1 Minutos\n4 - Cronometro (Iniciar e Finalizar)\n5 - Iniciar Temporizador");


  while (info != '5') {
    if (Serial.available() > 0) {
      info = Serial.read();


      if (info == '1') {
        tempo_escolhido += 1000;
        Serial.print("\n");
        Serial.print(tempo_escolhido / 60000);
        Serial.print(" Minutos e ");
        Serial.print((tempo_escolhido / 1000) % 60);
        Serial.println(" Segundos");
      }


      else if (info == '2') {
        tempo_escolhido += 10000;
        Serial.print("\n");
        Serial.print(tempo_escolhido / 60000);
        Serial.print(" Minutos e ");
        Serial.print((tempo_escolhido / 1000) % 60);
        Serial.println(" Segundos");
      }


      else if (info == '3') {
        tempo_escolhido += 60000;
        Serial.print("\n");
        Serial.print(tempo_escolhido / 60000);
        Serial.print(" Minutos e ");
        Serial.print((tempo_escolhido / 1000) % 60);
        Serial.println(" Segundos");
      }


      else if (info == '4') {
        tempo = 0;
        tempo = millis();
        digitalWrite(RELE_VALVULA, LOW);
        digitalWrite(RELE, LOW);
        volume_total = 0;


        while (info != '.') {
          sensor_fluxo();


          if (Serial.available() > 0) {
            info = Serial.read();


            if (info == '4') {
              digitalWrite(RELE, HIGH);
              digitalWrite(RELE_VALVULA, HIGH);
              tempo = millis() - tempo;
              Serial.print("\nTempo cronometrado:\n\n");
              Serial.print(tempo / 60000);
              Serial.print(" Minutos e ");
              Serial.print((tempo / 1000) % 60);
              Serial.println(" Segundos");
              info = '.';
              Serial.println("\nTemporizador:\n\n1 - 1 Segundo\n2 - 10 Segundos\n3 - 1 Minutos\n4 - Cronometro (Iniciar e Finalizar)\n5 - Iniciar Temporizador");
            }
          }
        }
      }


      else if (info == '5') {
        Serial.println("\nTemporizador iniciado");
      }
    }
  }


  tempo = millis() + tempo_escolhido;
  volume_total = 0;


  while (millis() < tempo) {
    digitalWrite(RELE_VALVULA, LOW);
    digitalWrite(RELE, LOW);
    sensor_fluxo();
  }


  digitalWrite(RELE, HIGH);
  digitalWrite(RELE_VALVULA, HIGH);
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
