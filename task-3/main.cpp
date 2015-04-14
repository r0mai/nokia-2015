
#include <iostream>
#include <boost/multiprecision/gmp.hpp>

using Number = boost::multiprecision::mpz_int;

int main() {
    Number lcm = 1;
    for (unsigned i = 1; i <= 10001; ++i) {
        lcm = boost::multiprecision::lcm(lcm, i);
    }

    std::cout << "--------------" << std::endl;
    std::cout << lcm / 2 / 8191 << std::endl;
    std::cout << "--------------" << std::endl;

    Number n = lcm / 2 / 8191;
    for (unsigned i = 1; i <= 10001; ++i) {
        if (n % i != 0) {
            std::cout << i << std::endl;
        }
    }
}
