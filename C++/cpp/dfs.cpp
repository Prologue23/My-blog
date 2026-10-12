#include "iostream"
using namespace std;

int main()
{
    int B_x,B_y,horse_x,horse_y;
    cin >> B_x >> B_y >> horse_x >> horse_y;
    bool d[B_x+1][B_y+1];
    unsigned long long dp[B_x+1][B_y+1];
    for (int i=0;i<=B_x;i++)
    {
        for (int j=0;j<=B_y;j++)
        {
            d[i][j]=0;
        }
    }
    d[horse_x][horse_y]=1;
    if (horse_x+1<=B_x)
    {
        if (horse_y+2<=B_y)
        {
            d[horse_x+1][horse_y+2]=1;
        }
        if (horse_y-2>=0)
        {
            d[horse_x+1][horse_y-2]=1;
        }
    }   
    if (horse_x-1>=0)
    {
        if (horse_y+2<=B_y)
        {
            d[horse_x-1][horse_y+2]=1;
        }
        if (horse_y-2>=0)
        {
            d[horse_x-1][horse_y-2]=1;
        }
    } 
    if (horse_x+2<=B_x)
    {
        if (horse_y+1<=B_y)
        {
            d[horse_x+2][horse_y+1]=1;
        }
        if (horse_y-1>=0)
        {
            d[horse_x+2][horse_y-1]=1;
        }
    }   
    if (horse_x-2>=0)
    {
        if (horse_y+1<=B_y)
        {
            d[horse_x-2][horse_y+1]=1;
        }
        if (horse_y-1>=0)
        {
            d[horse_x-2][horse_y-1]=1;
        }
    } 
    for (int i=0;i<=B_x;i++)
    {
        for (int j=0;j<=B_y;j++)
        {
            dp[i][j]=0;
        }
    }
    if (d[0][0]==0)
    {
        dp[0][0]=1;
    }
    else
    {
        dp[0][0]=0;
    }
    for (int i=0;i<=B_x;i++)
    {
        for (int j=0;j<=B_y;j++)
        {
            if (d[i][j]) continue;
            if(i>0) dp[i][j]+=dp[i-1][j];
            if(j>0) dp[i][j]+=dp[i][j-1];
        }
    }
    cout << dp[B_x][B_y];
    return 0;
}