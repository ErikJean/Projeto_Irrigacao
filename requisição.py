import requests
import json

url = "https://api.tomorrow.io/v4/weather/realtime?location=-10.9242, -37.1992&units=metric&apikey=IhykiZCUCknbJHLhz5hR003wdQimptY4"

headers = {"accept": "application/json"}

response = requests.get(url, headers=headers)

with open("dados_api.json", "w") as f:
    json.dump(response.json(), f, indent=4)

with open("dados_api.json", "r") as f:
    data = json.load(f)

print(data["data"]["time"])