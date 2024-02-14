import requests
import time

def get_ethereum_price():
    url = 'https://pro-api.coinmarketcap.com/v1/cryptocurrency/quotes/latest'
    parameters = {
        'symbol': 'ETH',
        'convert': 'USD'
    }
    headers = {
        'Accepts': 'application/json',
        'X-CMC_PRO_API_KEY': '120beb18-49c7-46c0-a537-c09dda90ed47'
    }
    response = requests.get(url, headers=headers, params=parameters)
    data = response.json()
    price = data['data']['ETH']['quote']['USD']['price']
    return price

user_input = input("How often do you want the price of eth to be checked (in seconds)? ")

x = int(user_input)

y = x/60

print(f"That is {y} minutes")

cycle_input = input("How many times do you want the price checked (if you would like it to repeat infinitely, please press any key other than a number)? ")

try:
    z = int(cycle_input)
except ValueError:
    while True:
        price = get_ethereum_price()
        print(f"The current price of Ethereum is ${price:.2f}")
        time.sleep(x) # sleep for x minutes
else: 
    for i in range(z):
        price = get_ethereum_price()
        print(f"The current price of Ethereum is ${price:.2f}")
        time.sleep(x) # sleep for x minutes




