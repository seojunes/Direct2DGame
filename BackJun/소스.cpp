#include <iostream>
using namespace std;
int main()
{
    int a, b, c;
    cin >> a >> b;
    cin >> c;
    b += c;
    a += b / 60;
    b = b % 60;
    

    if (a > 23)
    {
        a -= 24;
    }

    cout << a << " " << b;

    return 0;

}