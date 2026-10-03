initial_deposit = float(input("Enter the initial deposit amount: "))  #初始存款
cost_of_home = float(input("Enter the cost of the home: "))  #房价
portion_down_payment = 0.25 #首付比例
r_1 = 0    
r_2 = 1  
r = (r_1 + r_2) / 2  #利率
payment = cost_of_home * portion_down_payment  #首付金额
def diff(initial_deposit, r, payment):
    mount = initial_deposit
    for month in range(1,37):
        mount += mount * (r/12)
    return mount - payment
if  initial_deposit >= payment:
    print("It is enough to make the down payment.")
else:
    if initial_deposit * (1+r_2/12)**36 < payment:
        print("Not way to pay the down payment in three years.")
    else:
        i=1
        while abs(diff(initial_deposit, r, payment)) > 100:
            a = diff(initial_deposit, r, payment)
            if a > 100:
                r_2 = r
            elif a < -100:
                r_1 = r
            r = (r_1 + r_2) / 2
            i+=1
            if i>1000:
                print("No solution found.")
                break
        print("Best rate of return:", round(r*100,4),"%")



               

