where = input("Go left or right (l/r)?")
n = 3
while where == "r":
    n -= 1
    if n <= 0:
        print(":(")
    where = input("Go left or right (l/r)?")
else:
    print("You got out!") 