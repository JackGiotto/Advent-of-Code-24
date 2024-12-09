#include <iostream>
#include <fstream>
#include <string>
#include <vector>



void printVector(const std::vector<int>& ar) {
	for (size_t i = 0; i < ar.size(); i++) {
		std::cout << ar[i] << " ";
	}
}

std::vector<int> readFile(std::string filename) {
	std::fstream in;
	std::vector<int> ar;
	in.open(filename, std::ios::in);
	int i = 0;
	while (!in.eof()) {
		int c = in.get() - '0' + 1;
		//std::cout << c;
		for (int j = 0; j < c-1; j++) {
			ar.push_back(i);
		}
		c = in.get() - '0' + 1;
		for (int j = 0; j < c-1; j++) {
			ar.push_back(-1);
		}
		i++;
	}

	in.close();
	return ar;
}

void useFreeSpace(std::vector<int> &ar) {
	size_t old_start = 0;
	// starting from old_start changes time execution from and not from 0 changes mean time execution from 3s to 7ms
	for (size_t i = ar.size() - 1; i > 0; i--) {
		//std::cout << i << std::endl;
		if (ar[i] != -1) {
			for (size_t j = old_start; j < i; j++) {
				if (ar[j] == -1 ) {
					std::swap(ar[i], ar[j]);
					old_start = j;
					break;
				}
			}
		}
	}
}

void deleteEmpty(std::vector<int> &ar) {
	int i = 0;
	while (ar[i] != -1) {
		i++;
	}
	ar.resize(i);
	ar.shrink_to_fit();
}

long int calcCheckSum(const std::vector<int>& ar) {
	long int sum =0;
	for (size_t i = 1; i < ar.size(); i++) {
		sum += ar[i] * i;
	}
	return sum;
}

int main(int argc, char* argv[]) {
	std::vector<int> ar = readFile("input.txt");
	//printVector(ar);
	useFreeSpace(ar);
	deleteEmpty(ar);
	//printVector(ar);

	long int checksum = calcCheckSum(ar);
	std::cout << "Checksum: " << checksum << std::endl;
	return 0;
}
