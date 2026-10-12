#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;
using ll = long long;

struct Person
{
    ll time;
    ll index;
};

int main()
{
    int n;
    cin >> n;
    vector<Person> people(n);
    for (int i = 0; i < n; i++)
    {
        cin >> people[i].time;
        people[i].index = i + 1;
    }
    for (int i = 0; i < n - 1; i++)
    {
        int min_pos = i;
        for (int j = i + 1; j < n; j++)
        {
            if (people[j].time < people[min_pos].time ||
                (people[j].time == people[min_pos].time && people[j].index < people[min_pos].index))
            {
                min_pos = j;
            }
        }
        if (min_pos != i)
            swap(people[i], people[min_pos]);
    }

    ll total_wait = 0;
    ll wait = 0;
    for (int i = 0; i < n; i++)
    {
        cout << people[i].index;
        if (i != n - 1) cout << " ";
        total_wait += wait;
        wait += people[i].time;
    }
    cout << endl;
    cout << fixed << setprecision(2) << (double)total_wait / n << endl;
    return 0;
}