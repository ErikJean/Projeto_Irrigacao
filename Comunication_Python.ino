// Para armazenar um tempo refente a execução do arduino
unsigned long tempo_gravado = 0;

// Variáveis para controle de dados
String mensagem; // Controle de dados
char caractere; // Controle de dados
bool irrigando = 0; // Irrigação desativada ou ativada


// Definicao das variáveis de fluxo e volume
float volume = 0;
float volume_irrigado;
float fluxo = 0;

// Definicao do fator de calibracao para conversao do valor lido
float FATOR_CALIBRACAO = 4.7;


void setup() {
  Serial.begin(9600);
}

void loop() {
  if(Serial.available()) { // Leitura da comunicação
    caractere = Serial.read();

    if(caractere == '*') {
      volume = mensagem.toFloat();
      mensagem = "";
    }

    else if (caractere == ';') {
      FATOR_CALIBRACAO = mensagem.toFloat();
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

  if((millis() - tempo_gravado) > 1000) { // Impressão no monitor serial
    Serial.print("<");
    Serial.print(irrigando);
    Serial.print(";");
    Serial.print(FATOR_CALIBRACAO);
    Serial.print(";");
    Serial.print(volume);
    Serial.print(";");
    Serial.print(volume_irrigado);
    Serial.println(">");

    tempo_gravado = millis();
  } 
}
