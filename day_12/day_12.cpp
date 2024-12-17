#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>


const void printMatrix(const std::vector<std::vector<char>>& matrix) {
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

int cornerValue(const std::vector<std::vector<char>>& matrix, int i, int j) {
	// check corner N
	char plant = matrix[i][j];

	char n = (i-1 < 0) ? plant+1 : matrix[i-1][j];

	char ne = (i-1 < 0 || j+1 >= matrix[i].size()) ? plant+1 : matrix[i-1][j+1];

	char e = (j+1 >= matrix[i].size()) ? plant+1 : matrix[i][j+1];

	char se = (i+1 >= matrix.size() || j+1 >= matrix[i].size()) ? plant+1 : matrix[i+1][j+1];

	char s = (i+1 >= matrix.size()) ? plant+1 : matrix[i+1][j];

	char sw = (i+1 >= matrix.size() || j - 1 < 0) ? plant+1 : matrix[i+1][j-1];

	char w = (j-1 < 0) ? plant+1 : matrix[i][j-1];

	char nw = (i-1 < 0 || j-1 < 0) ? plant+1 : matrix[i-1][j-1];

	if (n != plant && e != plant && s != plant && w != plant) {
		return 4;
	}


	int counter = 0;
	// Check NW corner
	if ((n != plant  && w != plant)) {
		counter++;
	}

	// Check NE corner
	if ((n != plant && e != plant)) {
		counter++;
	}

	// Check SE corner
	if ((s != plant  && e != plant)) {
		counter++;
	}

	// Check SW corner
	if ((s != plant  && w != plant)) {
		counter++;
	}

	if (ne != plant && n == plant && e == plant) {
		counter ++;
	}

	if (se != plant && s == plant && e == plant) {
		counter++;
	}

	if (sw != plant && s == plant && w == plant) {
		counter++;
	}

	if (nw != plant && n == plant && w == plant) {
		counter++;
	}

	return counter;
}

void calcValueForRegionEdge(const std::vector<std::vector<char>>& matrix, std::vector<std::vector<bool>>& checked, char region, int i, int j, int& perimeter, int& area) {
	checked[i][j] = true;
	area++;

	perimeter += cornerValue(matrix, i, j);

	if (i - 1 < 0 || matrix[i-1][j] != region) {
		//perimeter += cornerValue(matrix, i, j);
	} else if (!checked[i-1][j]) {
		calcValueForRegionEdge(matrix, checked, region, i-1, j, perimeter, area);
	}

	if (i + 1 >= matrix.size() || matrix[i+1][j] != region) {
		//perimeter += cornerValue(matrix, i, j);
	} else if (!checked[i+1][j]) {
		calcValueForRegionEdge(matrix, checked, region, i+1, j, perimeter, area);
	}

	if (j - 1 < 0 || matrix[i][j-1] != region) {
		//perimeter += cornerValue(matrix, i, j);
	} else if (!checked[i][j-1]) {
		calcValueForRegionEdge(matrix, checked, region, i, j-1, perimeter, area);
	}

	if (j + 1 >= matrix[i].size() || matrix[i][j+1] != region) {
		//perimeter += cornerValue(matrix, i, j);
	} else if (!checked[i][j+1]) {
		calcValueForRegionEdge(matrix, checked, region, i, j+1, perimeter, area);
	}
}

int countValue(const std::vector<std::vector<char>>& matrix, bool edges) {
	long int value = 0;
	std::vector<std::vector<bool>> checked(matrix.size(), std::vector<bool>(matrix[0].size(), false));
	for (size_t i = 0; i < matrix.size(); i++) {
		for (size_t j = 0; j < matrix[i].size(); j++) {
			if (!checked[i][j]) {
				char region = matrix[i][j];
				int perimeter = 0;
				int area = 0;
				if (edges) {
					calcValueForRegionEdge(matrix, checked, region, i, j, perimeter, area);
				} else {
					calcValueForRegion(matrix, checked, region, i, j, perimeter, area);
				}
				//std::cout << "The region " << region << " has price = "  <<  area << "*" << perimeter << "=" << area*perimeter << std::endl << std::endl;
				value += area*perimeter;
			}
		}
	}
	return value;
}

int main(int argc, char* argv[]) {
	const std::vector<std::vector<char>> matrix = readFile("input.txt");
	//printMatrix(matrix);

	// Challenge 1
	int value = countValue(matrix, false);
	std::cout << "The cost counting perimeter is: " << value << std::endl;

	// Challenge 2
	value = countValue(matrix, true);
	std::cout << "The cost counting edges is: " << value << std::endl;

	return 0;
}