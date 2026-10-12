#include "iostream"
using namespace std;

void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
    return;
}

void bubbleSort(int enemy_unit[], int Adjusted_score[],int decision_score[],int v[],int total)
{
    for (int i = 0; i < total - 1; i++)
    {
        for (int j = 0; j < total - i - 1; j++)
        {
            if (decision_score[j] < decision_score[j + 1])
            {
                swap(Adjusted_score[j], Adjusted_score[j + 1]);
                swap(enemy_unit[j], enemy_unit[j + 1]);
                swap(decision_score[j],decision_score[j+1]);
                swap(v[j],v[j+1]);
            }
            if ((decision_score[j]==decision_score[j+1])&&(Adjusted_score[j]<Adjusted_score[j+1]))
            {
                swap(Adjusted_score[j], Adjusted_score[j + 1]);
                swap(enemy_unit[j], enemy_unit[j + 1]);
                swap(decision_score[j],decision_score[j+1]);
                swap(v[j],v[j+1]);
            }
        }
    }
    return;
}

int Basic_score(int &a,int &b)
{
    if ((a==1||a==3||a==4||a==5||a==7||a==6)&&b==1) return 1000;
    if ((a==1&&b==2)||((a==3||a==4||a==5)&&(b==3||b==4||b==5))||(a==6&&b==2)) return 800;
    if ((a==1&&(b==3||b==4||b==5))||((a==3||a==4||a==5)&&b==9)) return 100;
    if (((a==3||a==4||a==5)&&b==10)||(a==7&&b==9)) return 50;
    if ((a==7&&b==2)||(a==6&&(b==3||b==4||b==5))) return 900;
    if ((a==1||a==6||a==7||a==8)&&b==10) return 1000000;
    if ((a==3||a==4||a==5)&&(b==2||b==7)) return 600;
    if ((a==7&&(b==3||b==4||b==5))||(a==6&&b==9)) return 500;
    if (a==1&&b==7) return 400;
    if ((a==1||a==8)&&b==9) return 999999;
    if ((a==7||a==6)&&b==7) return 200;
    else return 0;
}


int main()
{
    int total,our_unit,last_enemy,plan;
    cin >> total >> our_unit >> last_enemy >> plan;
    int Adjusted_score[total];
    int enemy_unit[total];
    int v[total];
    for (int i = 0; i<total;i++)
    {
        cin >> enemy_unit[i] >> v[i];
    }


    int count = 0;
    for (int i = 0; i < total; i++) 
    {
        bool valid = true;

        if ((our_unit >= 3 && our_unit <= 7) || (our_unit == 1 && enemy_unit[i] != 9 && enemy_unit[i] != 10)) 
        {
            if (v[i] < 50 || v[i] > 1000) 
            {                
                valid = false;
            }
        }

        if (our_unit == 8 && enemy_unit[i] != 9 && enemy_unit[i] != 10) 
        {
            valid = false;
        }

        if (valid)
        {
            enemy_unit[count] = enemy_unit[i];
            v[count] = v[i];
            count++;
        }
    }
    total = count;
    
    
    if (plan==0)
    {
        for (int i = 0; i < total; i++)
        {
            Adjusted_score[i] = Basic_score(our_unit,enemy_unit[i]);
        }
    }
    if (plan==1)
    {
        for (int i = 0; i < total; i++)
        {
            if (enemy_unit[i]==1||enemy_unit[i]==7)
            {
                Adjusted_score[i] = Basic_score(our_unit,enemy_unit[i]) + 200;
            }
            else
            {
                Adjusted_score[i] = Basic_score(our_unit,enemy_unit[i]);
            }
        }
    }
    if (plan==2)
    {
        for (int i = 0; i < total; i++)
        {
            if (enemy_unit[i] == 2)
            {
                Adjusted_score[i] = Basic_score(our_unit,enemy_unit[i]) + 200;
            }
            else
            {
                Adjusted_score[i] = Basic_score(our_unit,enemy_unit[i]);
            }
        }
    }
    if (plan==3)
    {
        for (int i = 0; i < total; i++)
        {
            if (enemy_unit[i]==3||enemy_unit[i]==4||enemy_unit[i]==5)
            {
                Adjusted_score[i] = Basic_score(our_unit,enemy_unit[i]) + 200;
            }
            else
            {
                Adjusted_score[i] = Basic_score(our_unit,enemy_unit[i]);
            }
        }
    }
    if (plan==4)
    {
        for (int i = 0; i < total; i++)
        {
            if (enemy_unit[i] == (10))
            {
                Adjusted_score[i] = Basic_score(our_unit,enemy_unit[i]) + 500;
            }
            else
            {
                Adjusted_score[i] = Basic_score(our_unit,enemy_unit[i]);
            }
        }
    }
    if (plan==5)
    {
        for (int i = 0; i < total; i++)
        {
            if (enemy_unit[i] == (9))
            {
                Adjusted_score[i] = Basic_score(our_unit,enemy_unit[i]) + 500;
            }
            else
            {
                Adjusted_score[i] = Basic_score(our_unit,enemy_unit[i]);
            }
        }
    }
    if (plan==6)
    {
        for (int i = 0; i < total; i++)
        {
            if (enemy_unit[i] == (7))
            {
                Adjusted_score[i] = Basic_score(our_unit,enemy_unit[i]) + 200;
            }
            else
            {
                Adjusted_score[i] = Basic_score(our_unit,enemy_unit[i]);
            }
        }
    }
    if (plan==7)
    {
        for (int i = 0; i < total; i++)
        {
            if (enemy_unit[i]!=1&&enemy_unit[i]!=10&&enemy_unit[i]!=9)
            {
                Adjusted_score[i] = Basic_score(our_unit,enemy_unit[i]) - 100;
            }
            else
            {
                Adjusted_score[i] = Basic_score(our_unit,enemy_unit[i]);
            }
        }
    }
    int decision_score[total];
    for (int i = 0; i < total; i++)
    {
        if (our_unit == 8)
        {
            decision_score[i] = Basic_score(our_unit,enemy_unit[i]);
        }
        else
        {
            decision_score[i] = (1000 - v[i]) + Adjusted_score[i];
        }
    }
    for (int i = 0; i < total; i++)
    {
        if ((enemy_unit[i] == last_enemy)&&last_enemy!=0)
        {
            decision_score[i] = decision_score[i] + 300;
        }
    }



    bubbleSort(enemy_unit,Adjusted_score,decision_score,v,total);



    cout << enemy_unit[0] << " " << v[0] << endl;
    for (int i=0;i<total;i++)
    {
        if (i==0) 
        {
            cout << enemy_unit[i];
        }
        else
        {
            cout << " " << enemy_unit[i];
        }
    }
    cout << endl;


    if (enemy_unit[0]==1)
    {
        cout << "FIRE HERO!!!";
    }
    if(enemy_unit[0]==2)
    {
        cout << "FIRE ENGINEER!!!";
    }
    if(enemy_unit[0]==3||enemy_unit[0]==4||enemy_unit[0]==5)
    {
        cout << "FIRE INFANTRY!!!";
    }
    if(enemy_unit[0]==(7))
    {
        cout << "FIRE SENTRY!!!";
    }
    if(enemy_unit[0]==(9))
    {
        cout << "FIRE BASE!!!";
    }
    if(enemy_unit[0]==(10))
    {
        cout << "FIRE OUTPOST!!!";
    }
    return 0;
}