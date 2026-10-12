#include "iostream"
using namespace std;

//定义反序
int reverse(int x)
{
    int rev = 0;
    while (x != 0)
    {
        int pop = x % 10;
        x /= 10;
        rev = rev * 10 + pop;
    }
    return rev;
}

//检验素数
int Prime(int x)
{
    if (x < 2) return 0;
    for (int i = 2; i * i <= x; i++)
    {
        if (x % i == 0) return 0;
    }
    return 1;
}

int main()
{
    int M,N;
    cin >> M >> N;
    int j = 0;

    for (int i = M; i <= N; i++)
    {
        
        if (Prime(i) && Prime(reverse(i)) == 1)
        {
            if (j == 0)
            cout << i;
            if (j >= 1)
            cout << "," << i;
            j++;
        }
    }
    if (j == 0)
    cout << "No";
    return 0;
}