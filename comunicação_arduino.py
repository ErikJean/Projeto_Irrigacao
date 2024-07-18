import serial
import time

beginMarker = '<'
endMarker = '>'

mensagem_recebida = ''
mensagem_enviada = ''

volume = -1

while True: #Loop para a conexão com o Arduino
    try:  #Tenta se conectar, se conseguir, o loop se encerra
        arduino = serial.Serial('COM6', 9600)
        print('Arduino conectado')
        break
    except:
        pass
    
volume = float(input("Qual o volume a ser irrigado? "))

if(volume != -1):
    mensagem_enviada = str(volume) + "*" 
    arduino.write(mensagem_enviada.encode())
    arduino.flush()

while(True):
    while(arduino.inWaiting() > 0):
        mensagem_recebida += arduino.readline(1).decode()

    print(mensagem_recebida)
    mensagem_recebida = ""
    time.sleep(1)