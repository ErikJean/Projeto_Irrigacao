// Para armazenar um tempo refente a execução do arduino
unsigned long tempoGravado = 0;

// Variáveis para controle de dados
String mensagem;     // Controle de dados
char caractere;      // Controle de dados
bool irrigando = 0;  // Irrigação desativada ou ativada


// Definicao das variáveis de fluxo e volume
float volume = 0;
float volumeIrrigado;
float fluxo = 0;

// Definicao do fator de calibracao para conversao do valor lido
float fatorCalibracao = 4.7;


void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {  // Leitura da comunicação
    caractere = Serial.read();

    if (caractere == '*') {
      volume = mensagem.toFloat();
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

  print();

  while (irrigando == 1) {
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
      }

      else {
        mensagem.concat(caractere);
      }
    }

    print();
  }
}

void print() {
  if ((millis() - tempoGravado) > 1000) {  // Impressão no monitor serial
      Serial.print("<");
      Serial.print(irrigando);
      Serial.print(";");
      Serial.print(fatorCalibracao);
      Serial.print(" | ");
      Serial.print(volume);
      Serial.print(";");
      Serial.print(volumeIrrigado);
      Serial.println(">");

      tempoGravado = millis();
    }
}
