#include <iostream>
#include <cstdlib>

std::uint32_t b = 0;

void mysrand(std::uint32_t r)
{
    b = r;
}

std::int16_t myrand()
{
    b = b * 214013 + 2531011;
    return (b / 65536)% 32768;
}

int main()
{
    // prev 
    mysrand(2854395920);
    int z = myrand();
    std::cout << ::b << std::endl;
    int a = myrand(), b = myrand(), c = myrand(), d = myrand(), e = myrand();
    std::clog << z << " " << a << " " << b << " " << c << " " << d << " " << e << " " << std::endl;
    
    {
        srand(2854395920);
        int z = rand(), a = rand(), b = rand(), c = rand(), d = rand(), e = rand();
        std::clog << z << " " << a << " " << b << " " << c << " " << d << " " << e << " " << std::endl;

    }
    return 0;
}
