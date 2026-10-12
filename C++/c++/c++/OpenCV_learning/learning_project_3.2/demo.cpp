#include <iostream>
using namespace std;
using ll = long long;
int main() {
    double a = -100;
    for (ll i = 0; i < 100000; i++) {
        a = 1 + 1.0/a;
    }
    cout << a << endl;
    return 0;
}