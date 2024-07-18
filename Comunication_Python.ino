// Para armazenar um tempo refente a execução do arduino
unsigned long tempo_gravado = 0;

//Variáveis para controle de dados
String mensagem; // Controle de dados
char caractere; // Controle de dados
bool irrigando = 0; // Irrigação desativada ou ativada


//Definicao das variáveis de fluxo e volume
float volume = 0;
float volume_irrigado;
float fluxo = 0;


void setup() {
  Serial.begin(9600);
}

void loop() {
  noInterrupts();

  if(Serial.available()) {
    caractere = Serial.read();

    if(caractere == '*') {
      volume = mensagem.toFloat();
      mensagem = "";
    }

    else if(caractere == '-') {
      irrigando = mensagem.toInt();
      mensasagem = "";
    }

    else {
      mensagem.concat(caractere);
    }
  }

  if((millis() - tempo_gravado) > 1000) {
    Serial.flush();
    Serial.print("<");
    Serial.print(irrigando);
    Serial.print(";");
    Serial.print(volume);
    Serial.print(";");
    Serial.print(volume_irrigado);
    Serial.println(">");

    tempo_gravado = millis();
  } 
}
