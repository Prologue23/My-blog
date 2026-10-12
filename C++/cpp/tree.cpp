#include "iostream"
using namespace std;
//交换
void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
    return;
}

//冒泡排序
void bubbleSort(int starts[],int ends[],int m)
{
    for (int i = 0; i < m-1; i++)  //外层
    {
        for (int j=0; j < m - i - 1; j++)   //内层
        {
            if (starts[j] > starts[j+1])  //比较
            {
                swap(starts[j],starts[j+1]); //开始交换
                swap(ends[j],ends[j+1]);  //结束交换
            }
        }
    }
    return;
}

int main()
{
    int l,m;
    cin >> l >> m;

    //数组储存
    int starts[m];  
    int ends[m];

    //输入
    for (int i = 0; i < m; i++)
    {
        cin >> starts[i] >> ends[i];
    }

    bubbleSort(starts,ends,m);  //排序

    //合并
    int mergedstarts[m];
    int mergedends[m];
    int mergedcount = 0;

    
    mergedstarts[mergedcount] = starts[0];
    mergedends[mergedcount] = ends[0];
    mergedcount++;
    
    for (int i = 1; i < m; i++)
    {
        if (starts[i] <= mergedends[mergedcount - 1] + 1)
        {
            if (ends[i] > mergedends[mergedcount -1])
            {
                mergedends[mergedcount - 1] = ends[i];
            }
        }
        else
        {
            mergedstarts[mergedcount] = starts[i];
            mergedends[mergedcount] = ends[i];
            mergedcount++;
        }
    }
    
    //减去总数
    int declined = 0;
    for (int i = 0; i < mergedcount; i++)
    {
        declined += (mergedends[i] - mergedstarts[i] + 1);
    }
    cout << (l+1) - declined << endl;
    return 0;
}