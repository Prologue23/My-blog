#include <stdio.h>

int main()
{
	int number1,number2;
	number1 = 0;
	number2 = 0;
	
	
	printf("请输入数据");
	
	scanf("%d\n"&number1);
	scanf("%d\n"&number2);
	
	int change = number1 + number2;
	
	printf("结果为%d元\n");
	
	return 0;
}
