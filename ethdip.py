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

cycle_input = input("How many times do you want the price checked (if you would like it to repeat infinitely, please press any key other than a number)? ")

z = int(cycle_input)

prices = [0] * z

decr_input = input("What is your minimum target percentage decrease? ")

y = float(decr_input)

incr_input = input("What is your minimum target percentage increase? ")

a = float(incr_input)

for i in range(z):
    prices[i] = get_ethereum_price()
    print(f"The current price of Ethereum is ${prices[i]:.2f}")
    time.sleep(x) # sleep for x minutes
    if (i > 0):
        change = prices[i] - prices[i-1]
        perchange = (change/prices[i-1])*100
        if (perchange < 0):
            nperchange = perchange * (-1)
            if (nperchange >= y):
                print(f"Target decrease detetcted!")
                
        elif (perchange > 0):
            if (perchange >= a):
                print(f"Target increase detected!")
                
                
                

      
            
            
            
             

