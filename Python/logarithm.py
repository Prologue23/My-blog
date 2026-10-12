num1 = int(input("输入底数："))
num2 = int(input("输入真数："))
num3 = 2
e = 2.718281828459045
x1 = 1.0
x2 = x1 + num1/e**x1 - 1
y1 = 1.0
y2 = y1 + num2/e**y1 - 1
while abs(x2-x1) and abs(y2-y1)  > 0.000001:
    x1 = x2
    x2 = x1 + num1/e**x1 - 1
    y1 = y2
    y2 = y1 + num2/e**y1 - 1
print(f"log_{num1} {num2} = {y2/x2:6f}")