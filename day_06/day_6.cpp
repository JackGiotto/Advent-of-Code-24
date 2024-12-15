#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

enum direction {
	NORTH,
	EAST,
	SUD,
	WEST
};

void printVector(const std::vector<std::vector<char>>& matrix) {
	for (const auto& row : matrix) {
		for (const auto& elem : row) {
			std::cout << elem << "";
		}
		std::cout << std::endl;
	}
}

struct collision {
	int obstacle[2];
	enum direction d;
};

struct Guard {
	enum direction d;
	int starting_position[2];
	int position[2];
	int nrows;
	int counter = 1;
	int ncolumns;

	void turn() {
		switch (d) {
			case NORTH:
				d = EAST;
				return;
			case EAST:
				d = SUD;
				return;
			case SUD:
				d = WEST;
				return;
			case WEST:
				d = NORTH;
				return;
		}
	}

	short int move(std::vector<std::vector<char>>& matrix) {
		int nextx;
		int nexty;
		switch (d) {
			case NORTH:
				nextx = position[1];
				nexty = position[0] - 1;
				break;
			case EAST:
				nextx = position[1] + 1;
				nexty = position[0];
				break;
			case SUD:
				nextx = position[1];
				nexty = position[0] + 1;
				break;
			case WEST:
				nextx = position[1] - 1;
				nexty = position[0] ;
				break;
		}

		if (nextx >= ncolumns || nextx < 0 || nexty >= nrows || nexty < 0) {
			return -1;
		}

		if (matrix[nexty][nextx] == '#') {
			turn();
			return 2;
		}

		position[0] = nexty;
		position[1] = nextx;

		if (matrix[position[0]][position[1]] == '.') {
			matrix[position[0]][position[1]] = 'X';
			counter++;

			return 1;
		} else {
			return 0;
		}

	}

	void guardPatrol(std::vector<std::vector<char>>& matrix) {
		while (move(matrix) != -1) {
		}
	}

	int guardPatrolSearchingLoop(std::vector<std::vector<char>> &matrix) {
		short int check = 0;
		std::vector<struct collision> collisions;
		while ((check = move(matrix)) != -1) {
			if (check == 2) {
				struct collision c;
				c.obstacle[0] = position[0];
				c.obstacle[1] = position[1];
				c.d = d;
				bool found = false;
				for (const auto& col : collisions) {
					if (col.obstacle[0] == c.obstacle[0] && col.obstacle[1] == c.obstacle[1] && col.d == c.d) {
						found = true;
						break;
					}
				}
				if (!found) {
					collisions.push_back(c);
				} else {
					return 1;
				}
			}
		}
		return 0;
	}

	int checkEveryLoop(std::vector<std::vector<char>>& matrix, std::vector<std::vector<char>>& visited_matrix) {
		int count = 0;

		for (size_t i = 0; i < nrows; i++) {
			for (size_t j = 0; j < ncolumns; j++) {
				position[0] = starting_position[0];
				position[1] = starting_position[1];
				d = NORTH;

				std::vector<std::vector<char>> tmp_matrix = matrix;
				if (visited_matrix[i][j] == 'X') {
					tmp_matrix[i][j] = '#';
					if (guardPatrolSearchingLoop(tmp_matrix)) {
						count++;
					}
				}
			}
		}
		return count;
	}
};

std::vector<std::vector<char>> readFile(std::string filepath, struct Guard& g) {
	std::vector<std::vector<char>> matrix;
	std::fstream in;

	in.open(filepath, std::ios::in);

	std::string line;
	int i = 0, j = 0;
	while (std::getline(in, line)) {
		std::vector<char> new_vector;
		std::stringstream ss(line);
		char c;
		while (ss.get(c)) {
			if (c == '.' || c == '#' || c == '^') {
				if (c == '^') {
					c = 'G';
					g.d = NORTH;
					g.position[0] = i;
					g.position[1] = j;
					g.starting_position[0] = i;
					g.starting_position[1] = j;
				}
				new_vector.push_back(c);
				j++;
			}
		}
		if (i == 0) {
			g.ncolumns = j;
		}
		matrix.push_back(new_vector);
		i++;
		j = 0;
	}

	g.nrows = i;

	in.close();
	return matrix;
}

int main(int argc, char* argv[]) {
	// Challenge 1
	struct Guard g;
	std::vector<std::vector<char>> matrix = readFile("input.txt", g);
	std::vector<std::vector<char>> tmp_matrix = matrix;
	g.guardPatrol(tmp_matrix);
	std::cout << "visited " << g.counter << " distinct positions" << std::endl;

	// Challenge 2
	int numbers_of_loop = g.checkEveryLoop(matrix, tmp_matrix);
	std::cout << "Found " << numbers_of_loop << " possible loops" << std::endl;

	return 0;
}