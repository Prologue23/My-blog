#include "iostream"
using namespace std;

int main()
{int a,b,c,d,e,f,g,h,i = 0;
    cout << "输入3*3矩阵的9个元素,以空格隔开:"<< endl;
    cin >>a>>b>>c>>d>>e>>f>>g>>h>>i;
    cout << "你输入的矩阵为:" << endl;
    cout << a << "\t" << b << "\t" << c <<"\n";
    cout << d << "\t" << e << "\t" << f <<"\n";
    cout << g << "\t" << h << "\t" << i <<"\n";

    
    int devA = a*(e*i-f*h)-b*(d*i-f*g)+c*(d*h-e*g);
    if (devA == 0)
    {cout << "矩阵不可逆" << endl;}
    else if ( devA != 0 )
    {
        int A = ( e*i - f*h );
        int B = ( d*i - f*g );
        int C = ( d*h - e*g );
        int D = ( b*i - c*h );
        int E = ( a*i - c*g );
        int F = ( a*h - b*f );
        int G = ( b*f - c*e );
        int H = ( a*f - c*d );
        int I = ( a*e - b*d );
        
        cout << "矩阵的余子阵为:" << endl;
        cout << A << "\t" << D << "\t" << G <<"\n";
        cout << B << "\t" << E << "\t" << H <<"\n";
        cout << C << "\t" << F << "\t" << I <<"\n";

        cout << "矩阵的代数余子阵为：" << endl;
        cout << A  << "\t" << -D << "\t" << G  <<"\n";
        cout << -B << "\t" << E  << "\t" << -H <<"\n";
        cout << C  << "\t" << -F << "\t" << I  <<"\n";

        cout << "矩阵的伴随矩阵为：" << endl;
        cout << A  << "\t" << -B << "\t" << C  <<"\n";
        cout << -D << "\t" << E  << "\t" << -F <<"\n";
        cout << G  << "\t" << -H << "\t" << I  <<"\n";

        cout << "矩阵的逆矩阵为：" << endl;
        cout << "  "    << 1    << "\t" << A  << "\t" << -B << "\t" << C  <<"\n";
        cout << "-----" << "\t" << -D << "\t" << E  << "\t" << -F <<"\n";
        cout << "  "    << devA << "\t" << G  << "\t" << -H << "\t" << I  <<"\n";
    }
   return 0;
    
}