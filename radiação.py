import math
import json

def radiacao_liquida(): # Constantes
    a = 17.27
    b = 237.7
    G_sc = 0.0820  # constante solar em MJ/m²/min
    sigma = 4.903 * 10**-9  # constante de Stefan-Boltzmann em MJ/K⁴/m²/dia

    def calcular_t_dew(T, RH):
        gamma = (a * T) / (b + T) + math.log(RH / 100.0)
        T_dew = (b * gamma) / (a - gamma)
        return T_dew

    def calcular_Ra(latitude, J):
        phi = math.radians(latitude)
        dr = 1 + 0.033 * math.cos(2 * math.pi * J / 365)
        delta = 0.409 * math.sin(2 * math.pi * J / 365 - 1.39)
        omega_s = math.acos(-math.tan(phi) * math.tan(delta))
        Ra = (24 * 60 / math.pi) * G_sc * dr * (
            omega_s * math.sin(phi) * math.sin(delta) + 
            math.cos(phi) * math.cos(delta) * math.sin(omega_s)
        )
        return Ra

    def calcular_Rs(a, b, n, N, Ra):
        Rs = (a + b * (n / N)) * Ra
        return Rs

    def calcular_Rso(Ra, altitude):
        Rso = (0.75 + 2e-5 * altitude) * Ra
        return Rso

    def calcular_Rl(T_max, T_min, ea, Rs, Rso):
        T_max_K = T_max + 273.15
        T_min_K = T_min + 273.15
        Rl = sigma * ((T_max_K**4 + T_min_K**4) / 2) * (0.34 - 0.14 * math.sqrt(ea)) * (1.35 * (Rs / Rso) - 0.35)
        return Rl

    def calcular_ea(T_dew):
        ea = 0.6108 * math.exp((17.27 * T_dew) / (T_dew + 237.3))
        return ea

    def calcular_Rn(alpha, Rs, Rl):
        Rn = (1 - alpha) * Rs - Rl
        return Rn

    def calcular_N(latitude, J):
        phi = math.radians(latitude)
        delta = 0.409 * math.sin(2 * math.pi * J / 365 - 1.39)
        omega_s = math.acos(-math.tan(phi) * math.tan(delta))
        N = (24 / math.pi) * omega_s
        return N

    with open("dados_real.json", "r") as f:
        data = json.load(f)

    # Dados de entrada
    T = data["data"]["values"]["temperature"]  # Temperatura do ar em °C
    RH = data["data"]["values"]["humidity"]  # Umidade relativa em %
    latitude = -10.9242  # Latitude em graus
    J = 194  # Dia do ano (200º dia do ano)
    altitude = 29  # Altitude em metros

    with open("dados_1d.json", "r") as f:
        data = json.load(f)

    T_max = data["timelines"]["daily"][0]["values"]["temperatureMax"]  # Temperatura máxima diária em °C
    T_min = data["timelines"]["daily"][0]["values"]["temperatureMin"]  # Temperatura mínima diária em °C
    alpha = 0.23  # Albedo típico

    sunrise_time = [data["timelines"]["daily"][0]["values"]["sunriseTime"].split(":")[0].split("T")[1], data["timelines"]["daily"][0]["values"]["sunriseTime"].split(":")[1]]
    sunset_time = [data["timelines"]["daily"][0]["values"]["sunsetTime"].split(":")[0].split("T")[1], data["timelines"]["daily"][0]["values"]["sunsetTime"].split(":")[1]]
    n_minuto = ((int(sunset_time[0]) * 60) + int(sunset_time[1])) - ((int(sunrise_time[0]) * 60) + int(sunrise_time[1]))

    n = n_minuto / 60 # Número de horas de sol

    # Cálculos
    T_dew = calcular_t_dew(T, RH)
    Ra = calcular_Ra(latitude, J)
    N = calcular_N(latitude, J)
    Rs = calcular_Rs(0.25, 0.50, n, N, Ra)
    Rso = calcular_Rso(Ra, altitude)
    ea = calcular_ea(T_dew)
    Rl = calcular_Rl(T_max, T_min, ea, Rs, Rso)
    Rn = calcular_Rn(alpha, Rs, Rl)

    # Resultados
    #print(f"Temperatura do ponto de orvalho: {T_dew:.2f} °C")
    #print(f"Radiação solar incidente (Rs): {Rs:.2f} MJ/m²/dia")
    #print(f"Radiação de onda longa (Rl): {Rl:.2f} MJ/m²/dia")
    #print(f"Radiação líquida (Rn): {Rn:.2f} MJ/m²/dia")
    #print(f"Número máximo de horas de sol (N): {N:.2f} horas")

    return Rn