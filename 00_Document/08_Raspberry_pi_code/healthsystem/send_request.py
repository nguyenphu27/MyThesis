import requests

payload = {'username': 'value1', 'email': 'value2'}

r = requests.post("http://148.72.249.42:3000/upload", data=payload)
print(r.text)