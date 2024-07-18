unsigned long tempo_gravado = 0; // Para armazenar um tempo refente a execução do arduino
float volume = 0;
float volume_irrigado;
String mensagem; // Controle de dados
char caractere; // Controle de dados


void setup() {
  Serial.begin(9600);
}

void loop() {
  if(Serial.available()) {
    caractere = Serial.read();

    if(caractere == '*') {
      volume = mensagem.toFloat();
      mensagem = "";
    }

    else {
      mensagem.concat(caractere);
    }
  }

  if((millis() - tempo_gravado) > 1000) {
    Serial.flush();
    Serial.print("<");
    Serial.print(volume);
    Serial.print(";");
    Serial.print(volume_irrigado);
    Serial.println(">");

    tempo_gravado = millis();
  } 
}
