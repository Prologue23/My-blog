#include "iostream"
#include "queue"
using namespace std;

int main()
{
    int N,A,B;
    cin >> N >> A >> B;
    int K[N+1];
    queue<int> q;
    int dist[N+1];
    for (int i=1;i<=N;i++)
    {
        cin >> K[i];
        dist[i]=-1;
    }
    dist[A]=0;
    q.push(A);
    int up_new,down_new;
    while (!q.empty())
    {
        int now=q.front();
        q.pop();
        if(now==0)continue;
        if(now==B)break;
        
        up_new = now + K[now];
        if (up_new<=N&&dist[up_new]==-1)
        {
            dist[up_new]=dist[now]+1;
            q.push(up_new);
        }
        down_new = now - K[now];
        if (down_new>=1&&dist[down_new]==-1)
        {
            dist[down_new]=dist[now]+1;
            q.push(down_new);
        } 
    }
    cout << dist[B];
    return 0;
}