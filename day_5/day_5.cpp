#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

void printVector(const std::vector<short int>& numbers) {
    for (size_t i = 0; i < numbers.size(); i++) {
        std::cout << numbers[i] << " ";
    }
    std::cout << std::endl;
}

void printMap(const std::unordered_map<short int, std::vector<short int>>& map) {
    for (const auto& pair : map) {
        std::cout << "Key: " << pair.first << " Values: ";
        for (const auto& value : pair.second) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
}

std::unordered_map<short int, std::vector<short int>> readFile(const std::string& filename) {
    std::unordered_map<short int, std::vector<short int>> map;
    std::fstream in(filename, std::ios::in);
    if (!in) {
        std::cerr << "Error opening file" << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string part;
        std::getline(ss, part, '|');
        short int first = std::stoi(part);
        std::getline(ss, part, '|');
        short int second = std::stoi(part);
        map[second].push_back(first);
    }

    in.close();
    return map;
}

bool checkRow(const std::vector<short int>& numbers, const std::unordered_map<short int, std::vector<short int>>& map) {
    for (size_t i = 0; i < numbers.size(); i++) {
        for (size_t j = i + 1; j < numbers.size(); j++) {
            if (std::find(map.at(numbers[i]).begin(), map.at(numbers[i]).end(), numbers[j]) != map.at(numbers[i]).end()) {
                return false;
            }
        }
    }
    return true;
}

bool checkRowAndOrder(std::vector<short int>& numbers, const std::unordered_map<short int, std::vector<short int>>& map) {
	bool flag = false;
	for (size_t i = 1; i < numbers.size(); ++i) {
		short int key = numbers[i];
		size_t j = i;
		while (j > 0 && std::find(map.at(numbers[j - 1]).begin(), map.at(numbers[j - 1]).end(), key) != map.at(numbers[j - 1]).end()) {
			flag = true;
			numbers[j] = numbers[j - 1];
			j--;
		}
		numbers[j] = key;
	}
	return flag;
}

int sumMiddle(const std::string filename, const std::unordered_map<short int, std::vector<short int>>& map, bool order) {
    std::fstream in(filename, std::ios::in);
    if (!in) {
        std::cerr << "Error opening file" << std::endl;
        exit(1);
    }

    std::string line;
    int sum = 0;

    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string number;
        std::vector<short int> numbers;

        while (std::getline(ss, number, ',')) {
            numbers.push_back(std::stoi(number));
        }
		if (!order) {
			if (checkRow(numbers, map)) {
				sum += numbers[numbers.size() / 2];
			}
		} else {
			if (checkRowAndOrder(numbers, map)) {
				sum += numbers[numbers.size() / 2];
			}
		}
    }

    in.close();
    return sum;
}

int main(int argc, char* argv[]) {

    std::unordered_map<short int, std::vector<short int>> map = readFile("input1.txt");
    //printMap(map);
    int sum = sumMiddle("input2.txt", map, false);
    std::cout << "Sum of middle numbers: " << sum << std::endl;

	sum = sumMiddle("input2.txt", map, true);
    std::cout << "Sum of middle numbers ordered: " << sum << std::endl;

    return 0;
}