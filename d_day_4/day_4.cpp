#include <iostream>
#include <string>
#include <fstream>

void printMatrix(char** matrix, const int ncolumns, const int nrows) {
	for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncolumns; j++) {
			std::cout << matrix[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}

void count(bool c, const std::string filename, int& ncolumns, int& nrows) {
	std::fstream in;
	in.open(filename, std::ios::in);
	if (!in) {
		std::cout << "Error in file opening" << std::endl;
		exit(1);
	}
	int count = 1;
	std::string s;
	std::getline(in, s);
	ncolumns = s.size();
	while (std::getline(in, s)) {
		count++;
	}
	in.close();
	nrows = count;
}

char** readFile(const std::string filename, const int ncolumns, const int nrows) {
	char** matrix = new char*[nrows];

	std::fstream in;
	in.open(filename, std::ios::in);

	for (int i = 0; i < nrows; i++) {
		matrix[i] = new char[ncolumns];
		for (int j = 0; j < ncolumns; j++) {
			char c;
			in.get(c);
			while (c == '\n' || c == '\r') {
				in.get(c); // Skip newline characters
			}
			if (in) {
				matrix[i][j] = c;
			}
		}
	}

	in.close();
	return matrix;
}

bool checkXmas(char** matrix, const int ncolumns, const int nrows, int i, int j, const int direction) {
	int i_move;
	int j_move;
	switch(direction) {
		case 1:
			// up
			i_move = -1;
			j_move = 0;
			break;
		case 2:
			// up-right
			i_move = -1;
			j_move = 1;
			break;
		case 3:
			// right
			i_move = 0;
			j_move = 1;
			break;
		case 4:
			// below-right
			i_move = 1;
			j_move = 1;
			break;
		case 5:
			// below
			i_move = 1;
			j_move = 0;
			break;
		case 6:
			// below-left
			i_move = 1;
			j_move = -1;
			break;
		case 7:
			// left
			i_move = 0;
			j_move = -1;
			break;
		case 8:
			// up-left
			i_move = -1;
			j_move = -1;
			break;
	}
	int check1 = i + (i_move * 3);
	int check2 = j + (j_move * 3);
	if (check1 < 0 || check2 < 0 || check1 >= nrows || check2 >= ncolumns) {
		return false;
	}
	char xmas[3] = {'M', 'A', 'S'};
	i += i_move;
	j += j_move;

	for (int count = 0; count < 3; count++, i += i_move, j += j_move) {
		if (matrix[i][j] != xmas[count]) {
			return false;
		}
	}
	return true;
}

int countXmas(char** matrix, const int ncolumns, const int nrows) {
	int count = 0;
	for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncolumns; j++) {
			if (matrix[i][j] == 'X') {
				for (int direction = 1; direction <= 8; direction++) {
					if (checkXmas(matrix, ncolumns, nrows, i, j, direction)) {
						count++;
					}
				}
			}
		}
	}
	return count;
}

bool checkX_MAS(char** matrix, const int ncolumns, const int nrows, int i, int j) {
	if (i + 1 >= nrows || i - 1 < 0 || j + 1 > ncolumns || j-1 < 0) {
		return false;
	}
	else {
		if (
			(
				((matrix[i-1][j-1]=='M') && (matrix[i+1][j+1]=='S'))
				||
				((matrix[i-1][j-1]=='S') && (matrix[i+1][j+1]=='M'))
			)
			&&
			(
				((matrix[i-1][j+1]=='M') && (matrix[i+1][j-1]=='S'))
				||
				((matrix[i-1][j+1]=='S') && (matrix[i+1][j-1]=='M'))
			)
		) {
			return true;
		} else {
			return false;
		}
	}
}

int countX_MAS(char** matrix, const int ncolumns, const int nrows) {
	int count = 0;
	for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncolumns; j++) {
			if (matrix[i][j] == 'A') {
				if (checkX_MAS(matrix, ncolumns, nrows, i, j)) {
					count++;
				}
			}
		}
	}
	return count;
}

int main(int argc, char* argv[]) {
	// Challenge 1
	int ncolumns = 0;
	int nrows = 0;
	count(false, "input.txt", ncolumns, nrows);
	char** matrix = readFile("input.txt", ncolumns, nrows);
	int count = countXmas(matrix, ncolumns, nrows);
	std::cout << "counted: " << count << " XMAS" << std::endl;


	// Challenge 2
	count = countX_MAS(matrix, ncolumns, nrows);
	std::cout << "counted: " << count << " XMAS" << std::endl;

	for (int i = 0; i < nrows; i++) {
		delete[] matrix[i];
	}

	delete[] matrix;

	return 0;
}