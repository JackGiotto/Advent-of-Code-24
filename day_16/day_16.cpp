#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_set>
#include <vector>

int starting_x;
int starting_y;
int ending_x;
int ending_y;

constexpr int score_step = 1;
constexpr int score_turn = 1000;

struct Tile {
	int y;
	int x;
	int dir;
	constexpr Tile() : y(0), x(0), dir(0) {}
	constexpr Tile(int y, int x, int dir) : y(y), x(x), dir(dir) {}

	bool operator==(const Tile& t) const {
		return y == t.y && x == t.x && dir == t.dir;
	}

	bool operator<(const Tile& t) const {
		return y < t.y || (y == t.y && x < t.x) || (y == t.y && x == t.x && dir < t.dir);
	}
};

struct GraphHasher {
	std::size_t operator () (const Tile& t) const {
		return t.y;
	}
};

struct Comparator {
	bool operator()(const std::pair<Tile, int>& t1, const std::pair<Tile, int>& t2) const {
		return t1.second > t2.second;
	}
};

constexpr std::array<Tile, 4> directions{
	Tile(0, 1, 0),  // EAST
	Tile(1, 0, 1),	// SOUTH
	Tile(0, -1, 2), // WEST
	Tile(-1, 0, 3)  // NORTH
};

bool checkBoundaries(const Tile& tile, const std::vector<std::vector<char>>& matrix) {
	return (tile.y >= 0 && tile.y < matrix.size() && tile.x >= 0 && tile.x < matrix[0].size());
}

void readFile(std::string filename, std::vector<std::vector<char>>& matrix, Tile& start, Tile& end) {
	std::fstream in;
	in.open(filename, std::ios::in);
	if (!in) {
		std::cout << "Error on matrix file opening" << std::endl;
		exit(1);
	}

	std::string line;
	int i = 0;

	while (std::getline(in, line)) {
		int j = 0;
		std::vector<char> new_vector;
		for (char& c : line) {
			new_vector.push_back(c);
			if (c == 'S') {
				start.y = i;
				start.x = j;
			} else if (c == 'E') {
				end.y = i;
				end.x = j;
			}
			j++;
		}
		matrix.push_back(new_vector);
		i++;
	}

	in.close();
}

// Function for part 1
int findBestScore(const Tile& start, const Tile& end, std::vector<std::vector<char>>& matrix, int& direction_last) {
	std::priority_queue<std::pair<Tile, int>, std::vector<std::pair<Tile, int>>, Comparator> pq;
	std::unordered_set<Tile, GraphHasher> visited;
	pq.push({start, 0});
	while (!pq.empty()) {
		const auto [current, score] = pq.top();
		pq.pop();
		if (visited.find(current) != visited.end()) {
			continue;
		}
		visited.insert(current);
		if (current.y == end.y && current.x == end.x) {
			direction_last = current.dir;
			return score;
		}
		for (const auto& move : directions) {
			Tile new_tile;
			new_tile.y = current.y + move.y;
			new_tile.x = current.x + move.x;
			new_tile.dir = move.dir;

			if (!checkBoundaries(new_tile, matrix) || matrix[new_tile.y][new_tile.x] == '#') {
				continue;
			} else {
				if (current.dir == new_tile.dir) {
					pq.push({new_tile, score + 1});
				} else if (std::abs(current.dir - new_tile.dir) == 2) {
					pq.push({new_tile, score + (score_turn*2) + score_step});
				} else {
					pq.push({new_tile, score + score_turn + score_step});
				}
			}
		}
	}

	return -1;
}

std::vector<std::vector<std::array<int, 4>>> findMatrixForBestPaths(const Tile& start, const Tile& end, const std::vector<std::vector<char>>& matrix, const int best_score) {
	std::vector<std::array<int, 4>> tmp(matrix[0].size(), {{-1, -1, -1, -1}});
	std::vector<std::vector<std::array<int, 4>>> best_paths_matrix(matrix.size(), tmp);

	std::priority_queue<std::pair<Tile, int>, std::vector<std::pair<Tile, int>>, Comparator> pq;
	std::unordered_set<Tile, GraphHasher> visited;
	pq.push({end, 0}); // from end to start
	while (!pq.empty()) {
		const auto [current, score] = pq.top();
		pq.pop();
		if (visited.find(current) != visited.end()) {
			continue;
		}
		visited.insert(current);

		best_paths_matrix[current.y][current.x][current.dir] = score;

		Tile new_tile;
		auto move = directions[current.dir];
		new_tile.y = current.y - move.y;
		new_tile.x = current.x - move.x;

		if (!checkBoundaries(new_tile, matrix) || matrix[new_tile.y][new_tile.x] == '#') {
			continue;
		} else {
			for (int i = 0; i < 4; i++) {
				new_tile.dir = i;
				if (current.dir == new_tile.dir) {
					pq.push({new_tile, score + 1});
				} else if (std::abs(current.dir - new_tile.dir) == 2) {
					pq.push({new_tile, score + (score_turn*2) + score_step});
				} else {
					pq.push({new_tile, score + score_turn + score_step});
				}
			}
		}
	}
	return best_paths_matrix;
}

// return visited only for best paths (so all the tiles that are part of a best path)
std::unordered_set<Tile, GraphHasher> findCellsForBestPath(const Tile& start, const Tile& end, const std::vector<std::vector<char>>& matrix, const std::vector<std::vector<std::array<int, 4>>>& best_scores_only) {
	std::priority_queue<std::pair<Tile, int>, std::vector<std::pair<Tile, int>>, Comparator> pq;
	std::unordered_set<Tile, GraphHasher> visited;
	pq.push({start, best_scores_only[start.y][start.x][start.dir]});
	while (!pq.empty()) {
		const auto [current, score] = pq.top();
		pq.pop();
		if (visited.find(current) != visited.end()) {
			continue;
		}
		visited.insert(current);
		if (current.y == end.y && current.x == end.x) {
			continue;
		}
		for (const auto& move : directions) {
			Tile new_tile;
			new_tile.y = current.y + move.y;
			new_tile.x = current.x + move.x;
			new_tile.dir = move.dir;

			if (!checkBoundaries(new_tile, matrix) || matrix[new_tile.y][new_tile.x] == '#') {
				continue;
			} else {
				if (current.dir == new_tile.dir && best_scores_only[new_tile.y][new_tile.x][new_tile.dir] == score - 1) {
					pq.push({new_tile, score - score_step});
				} else if (std::abs(current.dir - new_tile.dir) == 2 && best_scores_only[new_tile.y][new_tile.x][new_tile.dir] == score - 2001) {
					pq.push({new_tile, score - ((score_turn*2) + score_step)});
				} else if (best_scores_only[new_tile.y][new_tile.x][new_tile.dir] == score - 1001) {
					pq.push({new_tile, score - (score_step + score_turn)});
				}
			}
		}
	}

	return visited;
}

int numberOfTiles(std::vector<std::vector<char>>& matrix, const Tile& start, const Tile& end, const int score) {
	std::vector<std::vector<std::array<int, 4>>> best_paths_matrix = findMatrixForBestPaths(start, end, matrix, score);
	std::unordered_set<Tile, GraphHasher> tiles_of_best_paths = findCellsForBestPath(start, end, matrix, best_paths_matrix);
	std::unordered_set<Tile, GraphHasher> tiles_of_best_paths_no_dup; // remove duplicates that are the same location but have a different direction
	for (const Tile& t : tiles_of_best_paths) {
		tiles_of_best_paths_no_dup.insert(Tile(t.y, t.x, 3)); // Fakes that direction is always NORTH
	}

	return tiles_of_best_paths_no_dup.size();
}

int main(int argc, char* argv[]) {
	std::vector<std::vector<char>> matrix;
	Tile start;
	Tile end;
	readFile("input.txt", matrix, start, end);

	// Part 1
	int last_direction = 0; // this is useful for part 2
	const int score = findBestScore(start, end, matrix, last_direction);
	std::cout << "Smallest score is: " << score << std::endl;


	// Part 2
	end.dir = last_direction;
	const int number_of_tiles = numberOfTiles(matrix, start, end, score);
	std::cout << "Number of tiles in at least one best path: " << number_of_tiles << std::endl;
	return 0;
}