#include "iostream"
using namespace std;

int main()
{
    unsigned long long N,M;
    cin >> N >> M;

    unsigned long long height[N];
    unsigned long long maxHeight = 0;

    for (int i = 0; i < N; i++)
    {
        cin >> height[i];
        if (height[i] > maxHeight)
        maxHeight = height[i];
    }
    
    unsigned long long low = 0, high = maxHeight,mid = 0,wood = 0;

    while (low <= high)
    {
        mid = (low + high) / 2;
        wood = 0;
        for (int i = 0; i < N; i++)
        {
            if (height[i] > mid)
            wood += height[i] - mid;
        }
        if (wood < M)
        {
            high = mid - 1;
        }
        if (wood >= M)
        {
            low = mid + 1;
        }
    }
    
    cout << high;
    return 0;
}