#include "iostream"
using namespace std;

int main()
{
    #define BMI_FAT 24
    #define BMI_OVER_FAT 28
    #define BMI_THIN 18.5
    
    float height,weight,bmi;
    cout << "请输入身高(cm)" << endl;
    cin >> height;
    height = height / 100;

    cout << "请输入体重(kg)" << endl;
    cin >> weight;
    
    bmi = weight / (height * height);

    cout << "您的BMI指数是" << bmi << endl;

    if(bmi >= BMI_FAT && bmi < BMI_OVER_FAT)
        cout << "您的体重超重" << endl;
        else if(bmi < BMI_FAT && bmi >= BMI_THIN)
            cout << "您的体重正常" << endl;
            else if( bmi < BMI_THIN )
                cout << "您的体重不足" << endl;
                else
                    cout << "您严重肥胖" << endl;
                    system("pause");
                    return 0;
}
