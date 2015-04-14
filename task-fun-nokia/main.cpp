#include <iostream>
#include <cassert>
#include <string>

int powi(int b, int e) {
    int res = 1;
    for (int i = 0; i < e; ++i) {
        res *= b;
    }
    return res;
}

int fun(const char *str) {
    std::string s(str);
    int pow = 0;
    int res = 1;
    for(char c : s) {
        res *= powi(int(c-'a'+1), ++pow);
        res %= 1000000;
    }
    return res;
}

// I have a thing for macros these days.
#define CALMARE_ASSERT(str, x) \
    std::cout << "fun(" str ") == " << x << " "; \
    (fun(str) == (x)) ? std::cout << "ok" : std::cout << "failed " << fun(str); \
    std::cout << std::endl;

int main() {
    CALMARE_ASSERT("a", 1);
    CALMARE_ASSERT("b", 2);
    CALMARE_ASSERT("c", 3);
    CALMARE_ASSERT("d", 4);
    CALMARE_ASSERT("e", 5);
    // ...
    CALMARE_ASSERT("z", 26);

    CALMARE_ASSERT("aa", 1);
    CALMARE_ASSERT("ab", 4);
    CALMARE_ASSERT("ac", 9);
    CALMARE_ASSERT("bb", 8);
    CALMARE_ASSERT("bc", 18);
    CALMARE_ASSERT("aaa", 1);
    CALMARE_ASSERT("aab", 8);
    CALMARE_ASSERT("aac", 27);
    CALMARE_ASSERT("abc", 108);
    CALMARE_ASSERT("bcd", 1152);
    CALMARE_ASSERT("bcddd", 989888);

    std::cout << fun("nokia") << std::endl;
}


