#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>

struct position {
	char c;
	bool antinode = false;
};

void printMatrix(const std::vector<std::vector<struct position>>& matrix) {
	for (size_t i = 0; i < matrix.size(); i++)
	{
		for (size_t j = 0; j < matrix[i].size(); j++)
		{
			std::cout << matrix[i][j].c;
		}
		std::cout << std::endl;
	}
}

std::vector<std::vector<struct position>> readFile(std::string filename) {
	std::vector<std::vector<struct position>> matrix;

	std::fstream in;
	in.open(filename, std::ios::in);

	if(!in) {
		std::cout << "Error on file opening" << std::endl;
		exit(1);
	}

	std::string line;
	while (std::getline(in, line)) {
		//std::stringstream ss(line);
		std::vector <struct position> tmp_vector;
		for (char& c: line) {
			struct position tmp;
			tmp.c = c;
			tmp_vector.push_back(tmp);
		}
		matrix.push_back(tmp_vector);
	}

	in.close();

	return matrix;
}

void addAntinode(std::vector<std::vector<struct position>>& matrix, size_t i, size_t j, size_t old_i, size_t old_j, bool propagation) {
	if (i >= 0 && i < matrix.size() && j >= 0 && j < matrix[i].size()) {
		if(!matrix[i][j].antinode && matrix[i][j].c != '#') {
			if (matrix[i][j].c == '.') {
				matrix[i][j].c = '#';
			} else {
				matrix[i][j].antinode = true;
			}
		}

		if (propagation) {
			int i_diff = (int)i - (int)old_i;
			int j_diff = (int)j - (int)old_j;

			int new_i = (int)i + i_diff;
			int new_j = (int)j + j_diff;

			addAntinode(matrix, (size_t)new_i, (size_t)new_j, i, j, propagation);
		}

	} else {
		return;
	}
}

void findOtherNodes(std::vector<std::vector<struct position>>& matrix, char antenna, size_t starting_i, size_t starting_j, bool propagation) {
	//std::cout << "antenna: " << antenna << std::endl;
	for (size_t i = starting_i; i < matrix.size(); i++) {
		for (size_t j = 0; j < matrix[i].size(); j++) {

			if (i != starting_i && j != starting_j) {
				if (matrix[i][j].c == antenna) {
					if (propagation) {
						matrix[i][j].antinode = true;
						matrix[starting_i][starting_j].antinode = true;
					}
					size_t new_i = i > starting_i ? i - (starting_i - i) : i + (i - starting_i);
					size_t new_i_2 = i > starting_i ? starting_i + (starting_i - i) : starting_i - (i - starting_i);
					size_t new_j = j > starting_j ? j - (starting_j - j) : j + (j - starting_j);
					size_t new_j_2 = j > starting_j ? starting_j + (starting_j - j) : starting_j - (j - starting_j);
					addAntinode(matrix, new_i, new_j, i, j, propagation);
					addAntinode(matrix, new_i_2, new_j_2, starting_i, starting_j, propagation);
				}
			}
		}
	}
}

void findAntennas(std::vector<std::vector<struct position>>& matrix, bool propagation) {
	for (size_t i = 0; i < matrix.size(); i++) {
		for (size_t j = 0; j < matrix[i].size(); j++) {
			if (matrix[i][j].c != '.' && matrix[i][j].c != '#') {
				char c = matrix[i][j].c;
				findOtherNodes(matrix, c, i, j, propagation);
			}
		}
	}
}

int countAntinodes(const std::vector<std::vector<struct position>>& matrix) {
	int count = 0;
	for (size_t i = 0; i < matrix.size(); i++) {
		for (size_t j = 0; j < matrix[i].size(); j++) {
			if (matrix[i][j].c == '#' || matrix[i][j].antinode) {
				count++;
			}
		}
	}
	return count;
}

int main(int argc, char* argv[]) {
	std::vector<std::vector<struct position>> matrix = readFile("input.txt");

	// Challenge 1

	//printMatrix(matrix);
	findAntennas(matrix, false);
	//printMatrix(matrix);
	int count = countAntinodes(matrix);
	std::cout << "Number of antinodes: " << count << std::endl;


	// challenge 2
	findAntennas(matrix, true);
	//printMatrix(matrix);
	count = countAntinodes(matrix);
	std::cout << "Number of antinodes with propagation: " << count << std::endl;

	return 0;
}