#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <climits>

const int N_ROWS = 103;
const int N_COLUMNS = 101;

/*
// FOR THE EXAMPLE
	const int N_ROWS = 7;
	const int N_COLUMNS = 11;
*/

const int OUT_OF_AREA_ROW = N_ROWS / 2;
const int OUT_OF_AREA_COLUMN = N_COLUMNS / 2;

struct Robot {
	int position[2];
	int speed[2];

	Robot(int position_x, int position_y, int speed_x, int speed_y) {
		position[0] = position_x;
		position[1] = position_y;
		speed[0] = speed_x;
		speed[1] = speed_y;
	}
};

void printRobots(const std::vector<Robot>& r) {
	for (size_t i = 0; i < r.size(); i++) {
		std::cout << "p=" << r[i].position[0] << "," << r[i].position[1] << " v=" << r[i].speed[0] << "," << r[i].speed[1] << std::endl;
	}
}

std::vector<Robot> readFile(std::string filename) {
	std::fstream in;
	in.open(filename, std::ios::in);
	if (!in) {
		std::cout << "Error on file opening" << std::endl;
		exit(1);
	}

	std::string pattern_string = R"(p=(\-?\d+),(\-?\d+) v=(\-?\d+),(\-?\d+))";
	std::regex pattern(pattern_string);

	long int price = 0;
	std::string s;
	std::vector<Robot> robots;
	while (std::getline(in, s)) {
		if (s != "\n") {
			std::smatch matches;
			std::string::const_iterator search_start(s.cbegin());
			while (std::regex_search(search_start, s.cend(), matches, pattern)) {
				robots.push_back(Robot(std::stoi(matches[1]), std::stoi(matches[2]), std::stoi(matches[3]), std::stoi(matches[4])));
				search_start = matches.suffix().first;
			}
		}
	}

	in.close();
	return robots;
}

void move(Robot& r) {
	int new_x = r.position[0] + r.speed[0];
	int new_y = r.position[1] + r.speed[1];

	if (new_x < 0) {
		new_x += N_COLUMNS;
	} else if (new_x >= N_COLUMNS) {
		new_x -= N_COLUMNS;
	}

	if (new_y < 0) {
		new_y += N_ROWS;
	} else if (new_y >= N_ROWS) {
		new_y -= N_ROWS;
	}

	r.position[0] = new_x;
	r.position[1] = new_y;
}

bool isATree(const std::vector<std::vector<int>>& matrix) {
	int count_digits = 0;
	for (size_t i = matrix.size()-1; i >= OUT_OF_AREA_ROW; i--) {
		for (size_t j = 0; j < OUT_OF_AREA_COLUMN; j++) {
			if (matrix[i][j] == 0) {
				count_digits = 0;
			} else {
				count_digits++;
			}

			if (count_digits >= 10) {
				return true;
			}
		}
	}
	return false;
}

int findTree(std::vector<Robot>& robots, int seconds = INT_MAX) {
	std::vector<int> tmp_vec(N_COLUMNS, 0);
	std::vector<std::vector<int>> matrix(N_ROWS, tmp_vec);

	for (size_t i = 0; i < seconds; i++) {
		for (size_t j = 0; j < robots.size(); j++) {
			if (i != 0) {
				matrix[robots[j].position[1]][robots[j].position[0]]--;
			}
			move(robots[j]);
			matrix[robots[j].position[1]][robots[j].position[0]]++;
		}
		if (isATree(matrix)) {
			return i;
		}
	}
	return 0;
}

std::vector<std::vector<int>> createMatrix(std::vector<Robot>& robots, int seconds = 100) {
	std::vector<int> tmp_vec(N_COLUMNS, 0);
	std::vector<std::vector<int>> matrix(N_ROWS, tmp_vec);

	for (size_t i = 0; i < robots.size(); i++) {
		for (int j = 0; j < seconds; j++) {
			move(robots[i]);
		}
		matrix[robots[i].position[1]][robots[i].position[0]]++;
	}
	return matrix;
}

long int countSafetyFactor(const std::vector<std::vector<int>>& matrix) {
	long int count_fac_areas[] = {0, 0, 0, 0};

	for (size_t i = 0; i < matrix.size(); i++) {
		for (size_t j = 0; j < matrix[i].size(); j++) {
			if (i != OUT_OF_AREA_ROW && j != OUT_OF_AREA_COLUMN)
				if (i < OUT_OF_AREA_ROW && j < OUT_OF_AREA_COLUMN) {
					count_fac_areas[0] += matrix[i][j];
				} else if (i < OUT_OF_AREA_ROW && j > OUT_OF_AREA_COLUMN) {
					count_fac_areas[1] += matrix[i][j];
				} else if (i > OUT_OF_AREA_ROW && j < OUT_OF_AREA_COLUMN) {
					count_fac_areas[2] += matrix[i][j];
				} else if (i > OUT_OF_AREA_ROW && j > OUT_OF_AREA_COLUMN) {
					count_fac_areas[3] += matrix[i][j];
				}
		}
	}

	return count_fac_areas[0] * count_fac_areas[1] * count_fac_areas[2] * count_fac_areas[3];
}

int main(int argc, char* argv[]) {
	const std::vector<Robot> robots = readFile("input.txt");
	std::vector<Robot> robots_tmp = robots;
	//printRobots(robots);

	std::vector<std::vector<int>> matrix = createMatrix(robots_tmp);
	//printMatrix(matrix);
	long int safety_factor = countSafetyFactor(matrix);
	std::cout << "The vaule is: " << safety_factor << std::endl;

	robots_tmp = robots;
	int seconds = findTree(robots_tmp);
	std::cout << "Found a tree after " << seconds << " seconds" << std::endl;

	return 0;
}