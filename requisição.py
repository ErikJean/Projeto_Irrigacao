import requests
import json

url_real = "https://api.tomorrow.io/v4/weather/realtime?location=-10.9242, -37.1992&units=metric&apikey=IhykiZCUCknbJHLhz5hR003wdQimptY4"
url_1h = "https://api.tomorrow.io/v4/weather/forecast?location=-10.9242, -37.1992&timesteps=1h&units=metric&apikey=IhykiZCUCknbJHLhz5hR003wdQimptY4"
url_1d = "https://api.tomorrow.io/v4/weather/forecast?location=-10.9242, -37.1992&timesteps=1d&units=metric&apikey=IhykiZCUCknbJHLhz5hR003wdQimptY4"

headers = {"accept": "application/json"}

response = requests.get(url_real, headers=headers)

with open("dados_real.json", "w") as f:
    json.dump(response.json(), f, indent=4)

response = requests.get(url_1h, headers=headers)

with open("dados_1h.json", "w") as f:
    json.dump(response.json(), f, indent=4)

response = requests.get(url_1d, headers=headers)

with open("dados_1d.json", "w") as f:
    json.dump(response.json(), f, indent=4)

# with open("dados_api.json", "r") as f:
#    data = json.load(f)

# print(data["data"]["time"])