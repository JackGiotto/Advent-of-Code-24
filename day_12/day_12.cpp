#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

int ntwosides =0;

void printMatrix(const std::vector<std::vector<char>>& matrix) {
	for (size_t i = 0; i < matrix.size(); i++) {
		for (size_t j = 0; j < matrix[i].size(); j++) {
			std::cout << matrix[i][j];
		}
		std::cout << std::endl;
	}
}

std::vector<std::vector<char>> readFile(std::string filepath) {
	std::vector<std::vector<char>> matrix;
	std::fstream in;

	in.open(filepath, std::ios::in);

	std::string line;

	while (std::getline(in, line)) {
		std::vector<char> new_vector;
		std::stringstream ss(line);
		char c;
		while (ss.get(c)) {
			if (c >= 'A' && c <= 'Z')
				new_vector.push_back(c);
		}
		matrix.push_back(new_vector);
	}

	in.close();
	return matrix;
}

void calcValueForRegion(const std::vector<std::vector<char>>& matrix, std::vector<std::vector<bool>>& checked, char region, int i, int j, int& perimeter, int& area) {
	checked[i][j] = true;
	area++;

	if (i - 1 < 0 || matrix[i-1][j] != region) {
		perimeter++;
	} else if (!checked[i-1][j]) {
		calcValueForRegion(matrix, checked, region, i-1, j, perimeter, area);
	}

	if (i + 1 >= matrix.size() || matrix[i+1][j] != region) {
		perimeter++;
	} else if (!checked[i+1][j]) {
		calcValueForRegion(matrix, checked, region, i+1, j, perimeter, area);
	}

	if (j - 1 < 0 || matrix[i][j-1] != region) {
		perimeter++;
	} else if (!checked[i][j-1]) {
		calcValueForRegion(matrix, checked, region, i, j-1, perimeter, area);
	}

	if (j + 1 >= matrix[i].size() || matrix[i][j+1] != region) {
		perimeter++;
	} else if (!checked[i][j+1]) {
		calcValueForRegion(matrix, checked, region, i, j+1, perimeter, area);
	}
}

long int countValue(const std::vector<std::vector<char>>& matrix) {
	long int value = 0;
	std::vector<std::vector<bool>> checked(matrix.size(), std::vector<bool>(matrix[0].size(), false));
	for (size_t i = 0; i < matrix.size(); i++) {
		for (size_t j = 0; j < matrix[i].size(); j++) {
			if (!checked[i][j]) {
				char region = matrix[i][j];
				int perimeter = 0;
				int area = 0;
				calcValueForRegion(matrix, checked, region, i, j, perimeter, area);
				std::cout << "The region " << region << " has price = "  <<  area << "*" << perimeter << "=" << area*perimeter << std::endl << std::endl;
				value += area*perimeter;
			}
		}
	}
	return value;
}

int cornerValue(const std::vector<std::vector<char>>& matrix, char region, int i, int j) {
	int counter = 4;

	if (i - 1 >= 0 && matrix[i-1][j] == region) {
		counter--;
	}
	if (i + 1 < matrix.size() && matrix[i+1][j] == region) {
		counter--;
	}
	if (j - 1 > 0 && matrix[i][j-1] == region) {
		counter--;
	}
	if (j + 1 < matrix.size() && matrix[i][j+1] == region) {
		counter--;
	}

	if (counter == 2) {
		ntwosides++;
		/*
		if (i - 1 >= 0 && matrix[i-1][j] == region) {
			counter--;
		} else {
			counter++;
		}
		if (j - 1 > 0 && matrix[i][j-1] == region) {
			counter--;
		}
		if (j + 1 < matrix.size() && matrix[i][j+1] == region) {
			counter++;
		}
		*/
	}
	if (counter == 1) {
		return 0;
	}
	return counter;
}

void calcValueForRegionEdge(const std::vector<std::vector<char>>& matrix, std::vector<std::vector<bool>>& checked, char region, int i, int j, int& perimeter, int& area) {
	checked[i][j] = true;
	area++;
	perimeter += cornerValue(matrix, region, i, j);

	if (i - 1 < 0 || matrix[i-1][j] != region) {
		// perimeter++;
	} else if (!checked[i-1][j]) {
		calcValueForRegionEdge(matrix, checked, region, i-1, j, perimeter, area);
	}

	if (i + 1 >= matrix.size() || matrix[i+1][j] != region) {
		// perimeter++;
	} else if (!checked[i+1][j]) {
		calcValueForRegionEdge(matrix, checked, region, i+1, j, perimeter, area);
	}

	if (j - 1 < 0 || matrix[i][j-1] != region) {
		// perimeter++;
	} else if (!checked[i][j-1]) {
		calcValueForRegionEdge(matrix, checked, region, i, j-1, perimeter, area);
	}

	if (j + 1 >= matrix[i].size() || matrix[i][j+1] != region) {
		// perimeter++;
	} else if (!checked[i][j+1]) {
		calcValueForRegionEdge(matrix, checked, region, i, j+1, perimeter, area);
	}
}

long int countValue2(const std::vector<std::vector<char>>& matrix) {
	long int value = 0;
	std::vector<std::vector<bool>> checked(matrix.size(), std::vector<bool>(matrix[0].size(), false));
	for (size_t i = 0; i < matrix.size(); i++) {
		for (size_t j = 0; j < matrix[i].size(); j++) {
			if (!checked[i][j]) {
				char region = matrix[i][j];
				int perimeter = 0;
				int area = 0;
				calcValueForRegionEdge(matrix, checked, region, i, j, perimeter, area);
				std::cout << "The region " << region << " has price = "  <<  area << "*" << perimeter << "=" << area*perimeter << std::endl << std::endl;
				value += area*(perimeter);
			}
		}
	}
	return value;
}

int main(int argc, char* argv[]) {
	std::vector<std::vector<char>> matrix = readFile("input2.txt");
	printMatrix(matrix);

	long int value = countValue2(matrix);

	std::cout << "The vaule is: " << value << " " << ntwosides << std::endl;


	return 0;
}