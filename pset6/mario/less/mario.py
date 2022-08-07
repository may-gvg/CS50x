from cs50 import get_int


def number(message):
    while True:
        try:
            user = int(input(message))
        except ValueError:
            continue
        else:
            return user


height = number("height: ")
while height < 1 or height > 8:
    height = int(input("height: "))


i = 0
while i < height:
    x = height-2
    a = 0
    while x >= i:
        print(" ", end="")
        x -= 1
    while a <= i:
        print("#", end="")
        a += 1
    print("")
    i += 1
