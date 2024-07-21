import serial
import time

beginMarker = '<'
endMarker = '>'

mensagem_recebida = ''
mensagem_enviada = ''

volume = -1
irrigando = 0
fator_calibracao = 4.7

while True: #Loop para a conexão com o Arduino
    try:  #Tenta se conectar, se conseguir, o loop se encerra
        arduino = serial.Serial('COM6', 9600)
        print('Arduino conectado')
        break
    except:
        pass
    
volume = float(input("Qual o volume a ser irrigado? "))
irrigando = int(input("Iniciar a irrigação (0 ou 1)? "))
fator_calibracao = float(input("Defina o fator de calibração: "))

if(volume != -1):
    mensagem_enviada = str(fator_calibracao) + ";" + str(volume) + "*" + str(irrigando) + "-"
    arduino.write(mensagem_enviada.encode())
    arduino.flush()

while(True):
    while(arduino.inWaiting() > 0):
        mensagem_recebida += arduino.readline(1).decode() # Tratar pois o ESP envia caracteres aleatórios

    print(mensagem_recebida)
    mensagem_recebida = ""
    time.sleep(2)
