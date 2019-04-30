import requests

base_url="www.server.com"
final_url="/{0}/friendly/{1}/url".format(base_url,any_value_here)

payload = {'number': 2, 'value': 1}
response = requests.post(final_url, data=payload)

print(response.text) #TEXT/HTML
print(response.status_code, response.reason) #HTTP