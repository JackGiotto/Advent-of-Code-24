#include <iostream>
#include <fstream>
#include <vector>
#include <string>

struct position {
	int i;
	int j;
};

void printMatrix(const std::vector<std::vector<short int>>& matrix) {
	for (size_t i = 0; i < matrix.size(); i++) {
		for (size_t j = 0; j < matrix[i].size(); j++) {
			std::cout << matrix[i][j];
		}
		std::cout << std::endl;
	}
}

std::vector<std::vector<short int>> readFile(std::string filename) {
	std::vector<std::vector<short int>> matrix;

	std::fstream in;
	in.open(filename, std::ios::in);

	if(!in) {
		std::cout << "Error on file opening" << std::endl;
		exit(1);
	}

	std::string line;
	while (std::getline(in, line)) {
		std::vector <short int> tmp_vector;
		for (char& c: line) {
			if (c >= '0' && c <= '9') {
				short int tmp = (int)c - '0';
				tmp_vector.push_back(tmp);
			}
		}
		matrix.push_back(tmp_vector);
	}

	in.close();

	return matrix;
}

int count(const std::vector<std::vector<short int>>& matrix, short int searching, int i, int j, std::vector<struct position>& positions, bool multiple_trails) {
	if (searching == matrix[i][j]) {
		if (searching == 9) {
			if (!multiple_trails) {
				for (size_t k = 0; k < positions.size(); k++) {
					if (positions[k].i == i && positions[k].j == j) {
						return 0;
					}
				}
				struct position tmp;
				tmp.i = i;
				tmp.j = j;
				positions.push_back(tmp);
			}
			return 1;
		}
		int score = 0;
		if (i+1 < matrix.size()) {
			score += count(matrix, searching+1, i+1, j, positions, multiple_trails);
		}
		if (i-1 >= 0) {
			score += count(matrix, searching+1, i-1, j, positions, multiple_trails);
		}
		if (j + 1 < matrix[i].size()) {
			score += count(matrix, searching+1, i, j+1, positions, multiple_trails);
		}
		if (j-1 >= 0) {
			score += count(matrix, searching+1, i, j-1, positions, multiple_trails);
		}
		return score;
	} else {
		return 0;
	}
}

int recursionGateway(const std::vector<std::vector<short int>>& matrix, short int searching, int i, int j, bool multiple_trails) {
	std::vector<struct position> positions;
	int score = 0;
	if (i+1 < matrix.size()) {
		score += count(matrix, searching+1, i+1, j, positions, multiple_trails);
	}
	if (i-1 >= 0) {
		score += count(matrix, searching+1, i-1, j, positions, multiple_trails);
	}
	if (j + 1 < matrix[i].size()) {
		score += count(matrix, searching+1, i, j+1, positions, multiple_trails);
	}
	if (j-1 >= 0) {
		score += count(matrix, searching+1, i, j-1, positions, multiple_trails);
	}
	return score;
}

int countTotalScore(const std::vector<std::vector<short int>>& matrix, bool multiple_trails) {
	int countScore = 0;
	int count;
	for (size_t i = 0; i < matrix.size(); i++) {
		for (size_t j = 0; j < matrix[i].size(); j++)
		{
			if (matrix[i][j] == 0)
				countScore += recursionGateway(matrix, 0, i, j, multiple_trails);
		}
	}
	return countScore;
}

int main(int argc, char* argv[]) {
	std::vector<std::vector<short int>> matrix = readFile("input.txt");
	//printMatrix(matrix);

	// Challenge 1
	int score = countTotalScore(matrix, false);
	std::cout << "Score: " << score << std::endl;

	// Challenge 2
	score = countTotalScore(matrix, true);
	std::cout << "Score with multiple trails: " << score << std::endl;
	return 0;
}