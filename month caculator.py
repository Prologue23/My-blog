yearly_salary = int(input("Enter your yearly salary: "))  #输入年薪
portion_saved = float(input("Enter the percent of your salary to save,as a decimal form:")) #输入储蓄比例
cost_of_house = int(input("Enter the cost of your dream house: ")) #输入房子总价
portion_down_payment = 0.25 * cost_of_house #首付金额
semi_annual_raise = float(input("Enter the semi-annual raise,as a decimal form:")) #输入半年加薪比例
amount_saved = 0.0 #初始储蓄金额
r = 0.05 #年化收益
monthly_salary = yearly_salary / 12 #月薪
monthly_saved = monthly_salary * portion_saved #月储蓄
month = 0 #月数计数器
while amount_saved < portion_down_payment:
    amount_saved += monthly_saved + amount_saved * r / 12
    month += 1
    if month % 6 == 0:
        monthly_salary += monthly_salary * semi_annual_raise
        monthly_saved = monthly_salary * portion_saved
print("Number of months:",month)
print("Number of years:",month/12)