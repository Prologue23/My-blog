#include "iostream"
using namespace std;

unsigned long long gcd(unsigned long long a,unsigned long long b)
{
    return b ? gcd(b,a%b) : a;
}

unsigned long long lcm(unsigned long long a,unsigned long long b)
{
    return a*b/gcd(a,b);
}

int main()
{
    unsigned long long a,b;
    cin >> a >> b;
    cout << gcd(a,b) << " " << lcm(a,b);
}