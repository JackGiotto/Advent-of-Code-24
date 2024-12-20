#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <sstream>

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


struct Tile {
	char tile_type;

	Tile(char c) {
		tile_type = c;
	}
};

void printMatrix(std::vector<std::vector<Tile>>& matrix, size_t x, size_t y) {
	for (size_t i = 0; i < matrix.size(); ++i) {
		for (size_t j = 0; j < matrix[i].size(); ++j) {
			if (i == y && j == x) {
				std::cout << "@ ";
			} else {
				std::cout << matrix[i][j].tile_type << " ";
			}
		}
		std::cout << std::endl;
	}
}

struct Robot {
	int x;
	int y;
	std::vector<direction> movements;

	void push(int dx, int dy, std::vector<std::vector<Tile>>& matrix, int tile_x, int tile_y) {
		int new_x = tile_x;
		int new_y = tile_y;

		while (matrix[new_y][new_x].tile_type == 'O') {
			new_x += dx;
			new_y += dy;
		}

		if (matrix[new_y][new_x].tile_type == '.') {
			matrix[new_y][new_x].tile_type = 'O';
			x = tile_x;
			y = tile_y;
			matrix[y][x].tile_type = '.';
		}
	}

	void move(direction d, std::vector<std::vector<Tile>>& matrix) {
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
				std::cout << "Default reached in function Move when checking Direction" << std::endl;
				break;
		}

		new_x += dx;
		new_y += dy;

		Tile moving_into = matrix[new_y][new_x];

		switch (moving_into.tile_type) {
			case '#':
				break;
			case 'O':
				push(dx, dy, matrix, new_x, new_y);
				break;
			case '.':
				x = new_x;
				y = new_y;
				break;
			default:
				std::cout << "Default reached in function Move when checking Tile" << std::endl;
				break;
		}
	}

	void patrol(std::vector<std::vector<Tile>>& matrix) {
		for (size_t i = 0; i < movements.size(); i++) {
			move(movements[i], matrix);
			/*
			std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << "moving " << movements[i] << std::endl;
			printMatrix(matrix, x, y);
			*/
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

Robot getData(std::string filename_matrix, std::string filename_movements, std::vector<std::vector<Tile>>& matrix, bool part_two = false) {
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
		std::vector<Tile> new_vector;
		for (char c : line) {
			if (part_two) {
				if (c == '#' || c == '.') {
					new_vector.push_back(Tile(c));
					new_vector.push_back(Tile(c));
					j += 2;
				} else if (c == 'O') {
					new_vector.push_back(Tile('['));
					new_vector.push_back(Tile(']'));
					j += 2;
				} else if (c == '@') {
					new_vector.push_back(Tile('.'));
					new_vector.push_back(Tile('.'));
					r.x = j;
					r.y = i;
					j += 2;
				}
			} else {
				if (c == '#' || c == '.' || c == 'O' ) {
					new_vector.push_back(Tile(c));
					j++;
				}
				if (c == '@') {
					new_vector.push_back(Tile('.'));
					r.x = j;
					r.y = i;
					j++;
				}
			}
		}
		matrix.push_back(new_vector);
		i++; // Increment row index after processing the entire line
	}
	in.close();

	r.movements = readFileMovements(filename_movements);
	return r;
}

unsigned long int getSumOfCoordinates(std::vector<std::vector<Tile>>& matrix) {
	unsigned long int coordinates_sum = 0;
	for (size_t i = 0; i < matrix.size(); i++) {
		for (size_t j = 0; j < matrix[i].size(); j++) {
			if (matrix[i][j].tile_type == 'O') {
				coordinates_sum += (i*100) + j;
			}
		}
	}
	return coordinates_sum;
}

int main(int argc, char* argv[]) {
	std::vector<std::vector<Tile>> matrix;
	Robot r = getData("input_test.txt", "input_test2.txt", matrix, true);
	printMatrix(matrix, r.x, r.y);
	//r.patrol(matrix);
	//unsigned long int coordinates_sum = getSumOfCoordinates(matrix);

	//std::cout << "The Score is: " << coordinates_sum << std::endl;

	return 0;
}