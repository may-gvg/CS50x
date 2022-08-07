
from cs50 import get_float


coins = 0


while True:
    change_dollar = get_float("Change (dollars): ")
    if change_dollar > 0:
        break


change = round(int(change_dollar * 100))


while change > 0:
    while change >= 25:
        coins += 1
        change -= 25
    while change >= 10:
        coins += 1
        change -= 10
    while change >= 5:
        coins += 1
        change -= 5
    while change >= 1:
        change -= 1
        coins += 1

print(coins)