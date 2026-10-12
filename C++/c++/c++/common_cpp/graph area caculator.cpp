#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

class shape
{
    public:
        virtual ~shape() {};
        virtual double area() const=0;
};
class Circle : public shape
{
    public: 
        Circle(int r) : radius(r) {}
        double area() const override
        {
            return 3.1415926535897932384626473787573 * radius * radius;
        }
    private:
        int radius;    
};
class Rectangle : public shape
{
    public:
        Rectangle(int l,int w) : length(l),width(w) {}
        double area() const override
        {
            return length * width;
        }
    private:
        int length,width;    
};
class Triangle : public shape
{
    public:
        Triangle(int a,int b,int c) : side_1(a),side_2(b),side_3(c) {}
        double area() const override
        {
            int p=(side_1+side_2+side_3)/2;
            return sqrt(p*(p-side_1)*(p-side_2)*(p-side_3));
        } 
    private:
    int side_1,side_2,side_3;
}; 
int main()
{
    int n;
    cin >> n;
    cout << endl;
    for (int i=0;i<n;i++)
    {
        string type;
        cin >> type;
        shape* ptr=nullptr;
        if (type=="Circle")
        {
            int r;
            cin >> r;
            ptr = new Circle(r);
        }
        else if (type=="Rectangle")
        {
            int l,w;
            cin >> l >> w;
            ptr = new Rectangle(l,w);
        }
        else if (type=="Triangle")
        {
            int a,b,c;
            cin >> a >> b >> c;
            ptr = new Triangle(a,b,c); 
        }
        if (ptr) 
        {
            cout << fixed << setprecision(2) << ptr->area() << endl;
            delete ptr;
        }
    }
    return 0;
}