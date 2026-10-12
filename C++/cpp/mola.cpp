#include "iostream"
#include "vector"
using namespace std;
using ll=long long;

int main() 
{
    ll k[21],c[21];
    k[1]=1; 
    c[1]=0;
    for (int i=2;i<=20;i++)
    {
        k[i]=k[i-1]+c[i-1];
        c[i]=k[i-1];
    }
    int T;
    cin>>T;
    vector<ll> a(T+1),b(T+1),n(T+1),res(T+1,-1);
    for (int i=1;i<=T;i++)
    {
        cin >>a[i]>>n[i]>>b[i];
        int p=(n[i]-c[a[i]])%k[a[i]];
        if (p==0) p=(n[i]-c[a[i]])/k[a[i]];
        else p=-1;
        if (p>=1 && p<=1000000)
        res[i]=k[b[i]]*p+c[b[i]];
        else res[i]=-1;
    }
    for (int i=1;i<=T;i++) cout<<res[i]<<endl;
    return 0;
}