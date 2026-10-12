#include "iostream"
using namespace std;

void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
    return;
}

//冒泡排序
void bubbleSort(int total_score[],int Chinese_score[],int student_ID[],int Math_score[],int English_score[],int n)
{
    for (int i = 0; i < n; i++)  //外层
    {
        for (int j=0; j < n - i - 1; j++)   //内层
        {
            if (total_score[j] < total_score[j+1])  //比较
            {
                swap(total_score[j],total_score[j+1]); 
                swap(Chinese_score[j],Chinese_score[j+1]);
                swap(student_ID[j],student_ID[j+1]);
                swap(Math_score[j],Math_score[j+1]);
                swap(English_score[j],English_score[j+1]);
            }
            else if (total_score[j] == total_score[j+1])
            {
                if (Chinese_score[j] < Chinese_score[j+1])
                {
                    swap(total_score[j],total_score[j+1]); 
                    swap(Chinese_score[j],Chinese_score[j+1]);
                    swap(student_ID[j],student_ID[j+1]);
                    swap(Math_score[j],Math_score[j+1]);
                    swap(English_score[j],English_score[j+1]);
                }
                else if (Chinese_score[j] == Chinese_score[j+1])
                {
                    if (student_ID[j] > student_ID[j+1])
                    {
                        swap(total_score[j],total_score[j+1]); 
                        swap(Chinese_score[j],Chinese_score[j+1]);
                        swap(student_ID[j],student_ID[j+1]);
                        swap(Math_score[j],Math_score[j+1]);
                        swap(English_score[j],English_score[j+1]);
                    }
                }
            }
            
        }
    }
    return;
}

int main()
{
    int n;
    cin >> n;
    
    int student_ID[n];
    int Chinese_score[n];
    int Math_score[n];
    int English_score[n];
    int total_score[n];

    for (int i = 0; i < n; i++)
    {
        cin >> Chinese_score[i] >> Math_score[i] >> English_score[i];
        student_ID[i] = i + 1;    
    }

    for (int i = 0; i < n; i++)
    {
        total_score[i] = Chinese_score[i] + Math_score[i] + English_score[i];
    }

    bubbleSort(total_score,Chinese_score,student_ID,Math_score,English_score,n);

    if (n >= 5)
    {
        for(int i = 0; i < 5; i++)
        {
        cout << student_ID[i] << " " << total_score[i] << endl;
        }
    }
    else if (n < 5)
    {
        for (int i = 0; i < n; i++)
        {
            cout << student_ID[i] << " " << total_score[i] << endl;
        }
    }
    return 0;
}