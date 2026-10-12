#include <iostream>
#include <string>
#include <vector>
using namespace std;

int dx(char a)
{
    if (a=='P') return 3;
    if (a=='p') return 2;
    if (a=='G') return 1;
    if (a=='g') return 0;
    if (a=='m') return 0;
    return 0;
}

int main()
{
    int class_number = 0;
    cin >> class_number;
    vector<vector<int>> all_outputs;
    for (int i = 0; i < class_number; i++)
    {
        string s;
        cin >> s;
        int length = s.length();
        vector<int> prefix_sum(length + 1, 0);
        for (int j = 0; j < length; j++)
        {
            prefix_sum[j + 1] = prefix_sum[j] + dx(s[j]);
        }
        int question = 0;
        cin >> question;
        vector<int> outputs;
        for (int l = 0; l < question; l++)
        {
            int start, end;
            cin >> start >> end;
            int sum = prefix_sum[end] - prefix_sum[start - 1];
            outputs.push_back(sum);
        }
        all_outputs.push_back(outputs);
    }
    for (const auto& outputs : all_outputs)
    {
        for (int val : outputs)
        {
            cout << val << endl;
        }
    }
    return 0;
}