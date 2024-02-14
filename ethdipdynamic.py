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

cycle_input = input("How many times do you want the price checked? ")

z = int(cycle_input)

decr_input = input("What is your minimum target percentage decrease? ")

y = float(decr_input)

incr_input = input("What is your minimum target percentage increase? ")

a = float(incr_input)

class EthNode:
    def __init__(self, data):
        self.data = data
        self.next = None
        
opening_price = 0
closing_price = 0

for i in range(z):
    if i == 0:
        head = EthNode(0)
        head.data = get_ethereum_price()
        opening_price = head.data
        print(f"The current price of Ethereum is ${head.data:.2f}")
    elif i == 1:
        tail = EthNode(1)
        tail.data = get_ethereum_price()
        head.next = tail
        print(f"The current price of Ethereum is ${tail.data:.2f}")
    else:
        NewEthNode = EthNode(i)
        tail.next = NewEthNode
        tail = NewEthNode
        tail.data = get_ethereum_price()
        head = head.next
        print(f"The current price of Ethereum is ${tail.data:.2f}")
    if i == (z-1):
        closing_price = tail.data
    
    if i > 0:
        change = tail.data - head.data
        perchange = (change/head.data)*100
        if perchange > 0:
            if perchange > a:
                print(f"Target increase detected!")
            
        elif perchange < 0:
            nperchange = perchange * (-1)
            if nperchange > y:
                print (f"Target decrease detected!")
        
    time.sleep(x) # sleep for x 
    
change_over_time = ((closing_price - opening_price)/opening_price)*100

print(f"The opening price for Ethereum was {opening_price}, and the closing price for Ethereum was {closing_price}")
print(f"The percentage change over the time period you had analyzed was {change_over_time}%")