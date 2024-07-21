#define RELE_BOMBA 2
#define RELE_VALVULA 4

//definicao do pino do sensor e de interrupcao
const int INTERRUPCAO_SENSOR = 15;  //interrupt = 0 equivale ao pino digital 2
const int PINO_SENSOR = 15;

//definicao da variavel de contagem de voltas
unsigned long contador = 0;

// Para armazenar um tempo refente a execução do arduino
unsigned long tempo_gravado = 0;

//definicao do fator de calibracao para conversao do valor lido
float FATOR_CALIBRACAO = 4.7;

//Variáveis para controle de dados
String mensagem; // Controle de dados
char caractere; // Controle de dados
bool irrigando = 0; // Irrigação desativada ou ativada

//Definicao das variáveis de fluxo e volume
float volume = 0;
float volume_irrigado = 0;
float fluxo = 0;


void setup() {
  Serial.begin(9600);
  pinMode(RELE_BOMBA, OUTPUT);
  pinMode(RELE_VALVULA, OUTPUT);
  digitalWrite(RELE_BOMBA, !HIGH);
  digitalWrite(RELE_VALVULA, !HIGH);
  pinMode(PINO_SENSOR, INPUT_PULLUP);
}

void loop() {
  digitalWrite(RELE_VALVULA, LOW);
  digitalWrite(RELE_BOMBA, LOW);

  if(Serial.available()) {
    caractere = Serial.read();

    if(caractere == '*') {
      volume = mensagem.toFloat();
      mensagem = "";
    }

    else if(caractere == '-') {
      irrigando = mensagem.toInt();
      mensagem = "";
    }

    else {
      mensagem.concat(caractere);
    }
  }

  if(irrigando == 1) {
    volume_irrigado = 0;
    tempo_gravado = 0;

    while(volume_irrigado < volume && irrigando == 1) {
      digitalWrite(RELE_VALVULA, HIGH);
      digitalWrite(RELE_BOMBA, HIGH);
      sensor_fluxo();
    }

    digitalWrite(RELE_VALVULA, LOW);
    digitalWrite(RELE_BOMBA, LOW);
  }
}

void sensor_fluxo() {
  if ((millis() - tempo_gravado) > 1000) {

    //desabilita a interrupcão para realizar a conversão do valor de pulsos
    detachInterrupt(INTERRUPCAO_SENSOR);

    //conversão do valor de pulsos para L/min
    fluxo = ((1000.0 / (millis() - tempo_gravado)) * contador) / FATOR_CALIBRACAO;

    //calculo do volume em L passado pelo sensor
    volume = fluxo / 60;

    //armazenamento do volume
    volume_irrigado += volume;

    Serial.print("<");
    Serial.print(irrigando);
    Serial.print(";");
    Serial.print(volume);
    Serial.print(";");
    Serial.print(volume_irrigado);
    Serial.println(">");

    //reinicializacão do contador de pulsos
    contador = 0;

    //atualizacão da variável tempo_antes
    tempo_gravado = millis();

    //contagem de pulsos do sensor
    attachInterrupt(digitalPinToInterrupt(INTERRUPCAO_SENSOR), contador_pulso, FALLING);
  }
}

void contador_pulso() {
  contador++;
}
