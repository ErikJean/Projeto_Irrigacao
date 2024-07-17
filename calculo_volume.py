# Função para calcular o volume de água necessário para irrigação
def calcular_volume_irrigacao(ETo, Kc, Eff, area):
    # Calcular a evapotranspiração da cultura (ETc)
    ETc = ETo * Kc
    
    # Ajustar pela eficiência do sistema de irrigação
    ETc_ajustada = ETc / Eff
    
    # Calcular o volume de água necessário em litros
    volume = ETc_ajustada * area * 10
    
    return volume

# Função para calcular o tempo de operação da bomba
def calcular_tempo_operacao(volume_necessario, frequencia_irrigacao, vazao_bomba):
    # Calcular o volume por irrigação
    volume_por_irrigacao = volume_necessario / frequencia_irrigacao
    
    # Calcular o tempo de operação em horas
    tempo_operacao = volume_por_irrigacao / vazao_bomba
    
    return tempo_operacao

# Valores fornecidos
ETo = 2.91  # Evapotranspiração de referência em mm/dia
Kc = 0.4    # Coeficiente da cultura
Eff = 0.8   # Eficiência do sistema de irrigação
area = 100  # Área da plantação em metros quadrados (exemplo)
frequencia_irrigacao = 2  # Quantas vezes por dia vai irrigar
vazao_bomba = 500  # Vazão da bomba em litros por hora (exemplo)

# Calcular o volume de água necessário
volume_necessario = calcular_volume_irrigacao(ETo, Kc, Eff, area)

# Calcular o tempo de operação da bomba
tempo_operacao = calcular_tempo_operacao(volume_necessario, frequencia_irrigacao, vazao_bomba)

print(f"Volume de água necessário para irrigar a plantação: {volume_necessario:.2f} litros por dia")
print(f"Tempo de operação da bomba por irrigação: {tempo_operacao:.2f} horas por irrigação")