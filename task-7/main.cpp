
#include <iostream>
#include <string>
#include <vector>

#include "Graph.hpp"

struct Transmission {
    unsigned source;
    unsigned target;
    unsigned size;
};

using Transmissions = std::vector<Transmission>;

int main() {
    std::string line;

    Graph graph = createGraph();
    Transmissions transmissions;
    while (std::getline(std::cin, line)) {
        Transmission t;
        std::cin >> t.source >> t.target >> t.size;
        transmissions.push_back(t);
    }

}
