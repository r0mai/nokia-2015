#include <iostream>
#include <cstdlib>

int main()
{
    int a = rand(), b = rand(), c = rand(), d = rand(), e = rand(), f=rand();

    while(b != 18981 || c != 12626 || d != 28782 || e!= 27874 || f != 17178)
    {
        a = b;
        b = c;
        c = d;
        d = e;
        e = f;
        f = rand();
    }
    std::cout << a << std::endl;
}
