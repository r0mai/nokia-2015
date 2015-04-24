#include <iostream>
#include <fstream>
#include <cmath>
#include <unordered_map>

int main() {
    std::unordered_map< int, int > sends;
    std::ifstream input_f("task7.txt");
    std::istream& input(input_f.is_open() ? input_f : std::cin);

    int from, to, bytes;
    while(input >> from >> to >> bytes)
        sends[from] += bytes;
    int max = 0;
    for(const auto& p : sends) {
        const int maybe = std::round(p.second * 8 / 1000.);
        if(maybe > max)
            max = maybe;
    }
    std::cout << max << std::endl;
}
