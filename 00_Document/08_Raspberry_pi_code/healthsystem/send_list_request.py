import requests
import json

with open('result.json') as data_file:
    Infor1 = json.load(data_file)
r = requests.post("http://148.72.249.42:3000/addinfor", data=Infor1)
print(r.text)
