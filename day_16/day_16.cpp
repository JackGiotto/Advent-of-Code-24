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
    Tile(0, 1, 0),
    Tile(1, 0, 1),
    Tile(0, -1, 2),
    Tile(-1, 0, 3)};

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

int search(const Tile& start, const Tile& end, std::vector<std::vector<char>>& matrix) {
    std::priority_queue<std::pair<Tile, int>, std::vector<std::pair<Tile, int>>, Comparator> pq;
    std::unordered_set<Tile, GraphHasher> visited;
    pq.push({start, 0});
    while (!pq.empty()) {
        const auto [current, score] = pq.top();
        pq.pop();
        if (visited.find(current) != visited.end()) continue;
        visited.insert(current);
        if (current.y == end.y && current.x == end.x) {
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

int main(int argc, char* argv[]) {
    std::vector<std::vector<char>> matrix;
    Tile start;
    Tile end;
    readFile("input.txt", matrix, start, end);
    std::vector<std::vector<bool>> visited(matrix.size(), std::vector<bool>(matrix[0].size(), false));

    const int score = search(start, end, matrix);
    std::cout << "Smallest score is: " << score << std::endl;

    return 0;
}