#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>

enum direction {
	NORTH,
	EAST,
	SOUTH,
	WEST
};

std::unordered_map<char, direction> DIRECTION_CONVERTER = {
	{'^', direction::NORTH},
	{'>', direction::EAST},
	{'v', direction::SOUTH},
	{'<', direction::WEST},
};


void printMatrix(std::vector<std::vector<char>>& matrix, size_t x, size_t y) {
	std::cout << "r\t";
	for (size_t j = 0; j < matrix[0].size(); ++j) {
		std::cout << j%10 << ' ';
	}
	std::cout << std::endl;
	bool flag = false;
	for (size_t i = 0; i < matrix.size(); ++i) {
		std::cout << i << "\t";
		for (size_t j = 0; j < matrix[i].size(); ++j) {
			if (i == y && j == x) {
				std::cout << "E";
			} else {
				std::cout << matrix[i][j] << " ";
			}
		}
		std::cout << std::endl;
	}
}

struct Robot {
	int x;
	int y;
	std::vector<direction> movements;

	void push(int dx, int dy, std::vector<std::vector<char>>& matrix, int tile_x, int tile_y) {
		int new_x = tile_x;
		int new_y = tile_y;

		while (matrix[new_y][new_x] == 'O') {
			new_x += dx;
			new_y += dy;
		}

		if (matrix[new_y][new_x] == '.') {
			matrix[new_y][new_x] = 'O';
			x = tile_x;
			y = tile_y;
			matrix[y][x] = '.';
		}
	}

	void pushLargeBoxHor(const int dx, std::vector<std::vector<char>>& matrix) {
		int i = x+dx;
		while (matrix[y][i] == '[' || matrix[y][i] == ']') {
			i += dx;
		};
		if (matrix[y][i] == '.') {
			for (int j = x+(dx*2); j != i; j += dx) {
				if (matrix[y][j] == '[') {
					matrix[y][j] = ']';
				} else {
					matrix[y][j] = '[';
				}
			}
			matrix[y][i] = (dx == 1) ? ']' : '[';
			matrix[y][x+dx] = '.';
			x += dx;
		}
	}

	bool isVertPushable(const int dy, int current_x, int current_y, std::vector<std::vector<char>>& matrix, std::unordered_map<int, std::vector<int>>& max_width, int& last_row) {
		if (matrix[current_y][current_x] == '#') {
			return false;
		}
		if (matrix[current_y][current_x] == '.') {
			return true;
		}

		if (dy == -1) {
			if (current_y < last_row) {
				last_row = current_y;
			}
		} else {
			if (current_y > last_row) {
				last_row = current_y;
			}
		}

		if (matrix[current_y][current_x] == '[') {
			if (max_width.find(current_y) == max_width.end()) {
				std::vector<int> tmp;
				tmp.push_back(current_x);
				tmp.push_back(current_x+1);
				max_width[current_y] = tmp;
			} else {
				if (max_width[current_y][0] > current_x) {
					max_width[current_y][0] = current_x;
				}
				if (max_width[current_y][1] < current_x+1) {
					max_width[current_y][1] = current_x+1;
				}
			}
			return isVertPushable(dy, current_x, current_y+dy, matrix, max_width, last_row) && isVertPushable(dy, current_x+1, current_y+dy, matrix, max_width, last_row);
		}
		if (matrix[current_y][current_x] == ']') {
			if (max_width.find(current_y) == max_width.end()) {
				std::vector<int> tmp;
				tmp.push_back(current_x-1);
				tmp.push_back(current_x);
				max_width[current_y] = tmp;
			} else {
				if (max_width[current_y][1] < current_x) {
					max_width[current_y][1] = current_x;
				}
				if (max_width[current_y][0] > current_x-1) {
					max_width[current_y][0] = current_x-1;
				}
			}
			return isVertPushable(dy, current_x, current_y+dy, matrix, max_width, last_row) && isVertPushable(dy, current_x-1, current_y+dy, matrix, max_width, last_row);
		}

		std::cout << "Reached a not wanted default" << std::endl;
		return false;
	}

	void pushLargeBoxVert(const int dy, std::vector<std::vector<char>>& matrix) {
		std::unordered_map<int, std::vector<int>> max_width;
		bool check = false;

		int last_row = -10000 * dy;
		if (matrix[y + dy][x] == '[') {
			check = isVertPushable(dy, x, y + dy, matrix, max_width, last_row) && isVertPushable(dy, x + 1, y + dy, matrix, max_width, last_row);
		} else if (matrix[y + dy][x] == ']') {
			check = isVertPushable(dy, x, y + dy, matrix, max_width, last_row) && isVertPushable(dy, x - 1, y + dy, matrix, max_width, last_row);
		}

		/*
		if (check) {
			std::cout << "pushable" << std::endl;
		} else {
			std::cout << "not pushable" << std::endl;
		}


		for (const auto& pair : max_width) {
			std::cout << "Row: " << pair.first << " -> Start: " << pair.second[0] << ", End: " << pair.second[1] << std::endl;
		}
		std::cout << "Last Row" << last_row << std::endl;
		*/
		if (check) {
			for (int i = last_row + dy; i != y+dy; i -= dy) {
				for (int j = max_width[i-dy][0]; j <= max_width[i-dy][1]; j++) {
					if (matrix[i-dy][j] == '[' || matrix[i-dy][j] == ']') {
						matrix[i][j] = matrix[i-dy][j];
						matrix[i-dy][j] = '.';
					}
				}
			}
			if (matrix[y + dy][x] == '[') {
				matrix[y + dy][x+1] = '.';
			} else if (matrix[y + dy][x] == ']') {
				matrix[y + dy][x-1] = '.';
			}
			matrix[y + dy][x] = '.';
			y += dy;
		}
	}

	void move(direction d, std::vector<std::vector<char>>& matrix) {
		int new_x = x;
		int new_y = y;
		int dx = 0;
		int dy = 0;
		switch (d) {
			case direction::NORTH:
				dy--;
				break;
			case direction::EAST:
				dx++;
				break;
			case direction::SOUTH:
				dy++;
				break;
			case direction::WEST:
				dx--;
				break;
			default:
				std::cout << "Default reached in function move when checking direction" << std::endl;
				break;
		}

		new_x += dx;
		new_y += dy;

		char moving_into = matrix[new_y][new_x];

		switch (moving_into) {
			case '#':
				break;
			case 'O':
				push(dx, dy, matrix, new_x, new_y);
				break;
			case '.':
				x = new_x;
				y = new_y;
				break;
			case '[':
			case ']':
				if (dy == 0) {
					pushLargeBoxHor(dx, matrix);
				} else {
					pushLargeBoxVert(dy, matrix);
				}
				break;
			default:
				std::cout << "Default reached in function move when checking char" << std::endl;
				break;
		}
	}


void patrol(std::vector<std::vector<char>>& matrix) {
		for (size_t i = 0; i < movements.size(); i++) {
			// std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << "moving " << movements[i] << " step: " << i << std::endl;
			move(movements[i], matrix);
			// printMatrix(matrix, x, y);
		}

		/*
		// cool thing that make you chose the path
		char c = 'x';
		do {
			std::cout << "Insert new movement: ";
			std::cin >> c;
			direction d = DIRECTION_CONVERTER[x];
			std::cout << "Moving " << d << std::endl;
			if (c != x) {
				move(DIRECTION_CONVERTER[c], matrix);
			}
			printMatrix(matrix, x, y);
		} while (c != 'x');
		*/
	}
};

std::vector<direction> readFileMovements(std::string filename) {
	std::fstream in;
	in.open(filename, std::ios::in);
	if (!in) {
		std::cout << "Error on movements file opening" << std::endl;
		exit(1);
	}

	std::vector<direction> vec;

	std::string line;
	while (std::getline(in, line)) {
		for (char c : line) {
			if (c == 'v' || c == '^' || c == '>' || c == '<') {
				vec.push_back(DIRECTION_CONVERTER[c]);
			}
		}
	}
	return vec;
}

Robot getData(std::string filename_matrix, std::string filename_movements, std::vector<std::vector<char>>& matrix, bool part_two = false) {
	std::fstream in;
	in.open(filename_matrix, std::ios::in);
	if (!in) {
		std::cout << "Error on matrix file opening" << std::endl;
		exit(1);
	}

	Robot r;
	std::string line;
	int i = 0;
	while (std::getline(in, line)) {
		int j = 0;
		std::vector<char> new_vector;
		for (char c : line) {
			if (part_two) {
				if (c == '#' || c == '.') {
					new_vector.push_back(c);
					new_vector.push_back(c);
					j += 2;
				} else if (c == 'O') {
					new_vector.push_back('[');
					new_vector.push_back(']');
					j += 2;
				} else if (c == '@') {
					new_vector.push_back('.');
					new_vector.push_back('.');
					r.x = j;
					r.y = i;
					j += 2;
				}
			} else {
				if (c == '#' || c == '.' || c == 'O' ) {
					new_vector.push_back(c);
					j++;
				} else if (c == '@') {
					new_vector.push_back('.');
					r.x = j;
					r.y = i;
					j++;
				}
			}
		}
		matrix.push_back(new_vector);
		i++;
	}
	in.close();

	r.movements = readFileMovements(filename_movements);
	return r;
}

unsigned long int getSumOfCoordinates(std::vector<std::vector<char>>& matrix, bool part2) {
	unsigned long int coordinates_sum = 0;
	char box = part2 ? '[' : 'O';
	for (size_t i = 0; i < matrix.size(); i++) {
		for (size_t j = 0; j < matrix[i].size(); j++) {
			if (matrix[i][j] == box) {
				coordinates_sum += (i*100) + j;
			}
		}

	}
	return coordinates_sum;
}

int main(int argc, char* argv[]) {
	// Part 1
	std::vector<std::vector<char>> matrix;
	Robot r = getData("input.txt", "input2.txt", matrix, false);
	r.patrol(matrix);
	//printMatrix(matrix);
	unsigned long int coordinates_sum = getSumOfCoordinates(matrix, false);

	std::cout << "The Score is: " << coordinates_sum << std::endl;

	// Part 2
	std::vector<std::vector<char>> matrix2;
	Robot r2 = getData("input.txt", "input2.txt", matrix2, true);
	r2.patrol(matrix2);
	//printMatrix(matrix2);
	coordinates_sum = getSumOfCoordinates(matrix2, true);

	std::cout << "The Score with new matrix is: " << coordinates_sum << std::endl;
	return 0;
}