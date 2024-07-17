import math
import json
import radiação

def penman_monteith(
    Rn,     # Radiação líquida na superfície da cultura (MJ/m²/dia)
    G,      # Fluxo de calor no solo (MJ/m²/dia)
    T,      # Temperatura do ar a 2 metros de altura (°C)
    u2,     # Velocidade do vento a 2 metros de altura (m/s)
    es,     # Pressão de saturação do vapor (kPa)
    ea,     # Pressão atual do vapor (kPa)
    Delta,  # Declividade da curva de pressão de vapor (kPa/°C)
    gamma   # Constante psicrométrica (kPa/°C)
):
    # Equação de Penman-Monteith da FAO
    ETo = (0.408 * Delta * (Rn - G) + gamma * (900 / (T + 273)) * u2 * (es - ea)) / (Delta + gamma * (1 + 0.34 * u2))
    return ETo

def pressao_saturacao_vapor(temperatura):
    # Fórmula de Tetens para a pressão de saturação do vapor
    e_s = 0.6108 * math.exp((17.27 * temperatura) / (temperatura + 237.3))
    return e_s

def calcular_pressao_vapor_atual(pressao_saturacao_vapor, umidade_relativa):
    # Calcular a pressão real do vapor de água (e) em hPa
    e = (pressao_saturacao_vapor * 10) * (umidade_relativa / 100.0)
    
    # Converter de hPa para kPa
    e_kPa = e / 10.0
    
    return e_kPa

def calcular_velocidade_vento_a_2m(u_z):
    # Calcular a velocidade do vento a 2 metros de altura
    u_2 = u_z * (math.log(2 / 0.1) / math.log(10 / 0.1))
    return u_2

def declividade_curva_pressao_vapor(pressao_saturacao_vapor, temperatura):
    # Calculando Δ
    delta = 4098 * pressao_saturacao_vapor / ((temperatura + 237.3) ** 2)
    
    return delta

def calcular_constante_psicrometrica(pressao_atmosferica):
    # Constantes
    cp = 1.013  # Calor específico do ar a pressão constante (kJ/kg°C)
    epsilon = 0.622  # Razão entre as massas molares de vapor d'água e ar seco
    lambda_ = 2.45  # Calor latente de vaporização (MJ/kg)

    # Calculando a constante psicrométrica (gamma)
    gamma = (cp * pressao_atmosferica) / (epsilon * lambda_)

    return gamma


with open("dados_real.json", "r") as f:
    data = json.load(f)

# Exemplo de uso
Rn = radiação.radiacao_liquida()    # MJ/m²/dia
G = 0.104    # MJ/m²/dia
T = data["data"]["values"]["temperature"]   # °C
u2 = calcular_velocidade_vento_a_2m(data["data"]["values"]["windSpeed"])   # m/s
es = pressao_saturacao_vapor(T) # kPa
ea = calcular_pressao_vapor_atual(es, data["data"]["values"]["humidity"]) # kPa
Delta = declividade_curva_pressao_vapor(es, T)  # kPa/°C
gamma = calcular_constante_psicrometrica(data["data"]["values"]["pressureSurfaceLevel"] / 10)  # kPa/°C

ETo = penman_monteith(Rn, G, T, u2, es, ea, Delta, gamma)
print(f"A evapotranspiração de referência (ETo) é: {ETo:.2f} mm/dia")