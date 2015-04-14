#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>

struct AdjacencyMatrix {
	std::vector<std::vector<bool>> adjacency;

	bool isSymmetric() const {
		for(std::size_t i=0; i<adjacency.size();++i) {
			for(std::size_t j=0; j<adjacency[i].size();++j) {
				if(adjacency[i][j] && !(adjacency[j][i])) {
					return false;
				}
			}
		}
		return true;
	}

};

int main() {

	std::string firstLine;
	std::getline(std::cin, firstLine);
	std::stringstream ss(firstLine);
	int currentIndex;
	while(ss.good()) {
		ss >> currentIndex;
	}
	const auto width = currentIndex;

	std::vector<bool> emptyVector;
	emptyVector.resize(width, false);

	AdjacencyMatrix matrix;

	for(int i=0; i<width; ++i) {
		int neighborId;
		std::cin>>neighborId;

		matrix.adjacency.push_back(emptyVector);
		for(int j=0; j<width; ++j) {
			int value;
			std::cin>>value;
			assert(value==0||value==1);
			matrix.adjacency[i][j] = static_cast<bool>(value);
		}
	}

	std::cerr<<"Input matrix is"<<(matrix.isSymmetric()?"":"not")
			<<" symmetric"<<std::endl;

}
