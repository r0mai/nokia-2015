#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <algorithm>

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

	bool areAllConnected() const {
		bool returnvalue = true;
		for(std::size_t row=0; row<adjacency.size(); ++row) {
			bool outsideConnectionPresent =
					std::any_of(adjacency[row].begin(), adjacency[row].end(),
							[](bool b){ return b; });
			if(!outsideConnectionPresent) {
				std::cerr<<"Node "<<row+1<<" has no connection"<<std::endl;
				returnvalue = false;
			}
		}
		return returnvalue;
	}

	bool isLeafNode(std::size_t nodeId) const {
		return std::count(
				adjacency[nodeId].begin(), adjacency[nodeId].end(), true) <= 1;
	}

	std::vector<std::size_t> getLeafNodes() const {
		std::vector<std::size_t> result;
		for(std::size_t i=0; i<adjacency.size(); ++i) {
			if(isLeafNode(i)) {
				result.push_back(i);
			}
		}
		return result;
	}

	void removeIndices(const std::vector<std::size_t>& indices) {
		for(const auto& index: indices) {
			removeIndex(index);
		}
	}

	void removeIndex(const std::size_t index) {
		for(auto& row: adjacency) {
			row.erase(row.begin()+index);
		}
		adjacency.erase(adjacency.begin()+index);
	}

	bool isNetworkDown() const {
		return std::all_of(adjacency.begin(), adjacency.end(), [](const std::vector<bool>& row) {
				return std::all_of(row.begin(), row.end(), [](bool b) { return b == false; });
		});
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
	std::cerr<<(matrix.areAllConnected()?"":"not ")
			<<"all nodes are connected in matrix"<<std::endl;
	const auto leafNodes = matrix.getLeafNodes();

	if(!leafNodes.size()) {
		std::cerr<<"Graph has no leaf nodes"<<std::endl;
	} else {
		std::cerr<<"Graph has "<<leafNodes.size()<<" leaf nodes"<<std::endl;
		for(const auto& nodeId: leafNodes) {
			std::cerr<<nodeId<<" ";
		}
		std::cerr<<std::endl;
	}

	std::cerr<<"Commencing search"<<std::endl;

	AdjacencyMatrix preprocessed = matrix;
	preprocessed.removeIndices(matrix.getLeafNodes());

	const std::size_t minimalAttackSize = 1;

	for(std::size_t i=minimalAttackSize; i<preprocessed.adjacency.size(); ++i) {
		std::cerr<<"Searching for attack with size="<<i<<std::endl;
		std::vector<bool> attackVector(width);
		for(std::size_t j=0; j<i; ++j) {
			attackVector[width-j-1] = true;
		}
		do {
#ifdef PRINTALL
			std::cerr<<"Current attack vector: "<<std::endl;
			for(int j=0; j<width; ++j) {
				if(attackVector[j])
					std::cerr<<j<<" ";
			}
			std::cerr<<std::endl;

			std::vector<std::size_t> attackIndices;
			for(int j=0; j<attackVector.size(); ++j) {
				if(attackVector[j])
					attackIndicies.push_back(j);
			}
			AdjacencyMatrix copy = preprocessed;
			copy.eraseIndicies(attackIndicies);
			if(copy.isNetworkDown()) {
				std::cerr<<"Attack successful with nodes:"<<std::endl;
				for(int j=0; j<width; ++j) {
					if(attackVector[j])
						std::cerr<<j<<" ";
				}
				std::cerr<<std::endl;
				std::cerr<<"Attack vector over"<<std::endl;
			}

#endif
		} while(std::next_permutation(attackVector.begin(), attackVector.end()));
	}

}
