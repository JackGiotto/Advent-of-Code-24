#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

void printVector(const std::vector<std::string>& vec) {
	for (size_t i = 0; i < vec.size(); i++) {
		std::cout << vec[i] << " ";
	}
	std::cout << std::endl;
}

std::vector<std::string> readFile(std::string filename) {
	std::vector<std::string> numbers;

	std::fstream in;
	in.open(filename, std::ios::in);

	if(!in) {
		std::cout << "Error on file opening" << std::endl;
		exit(1);
	}

	std::string number;

	while (in >> number) {
		numbers.push_back(number);
	}

	in.close();
	return numbers;
}

std::string checkZeros(std::string number) {
	int tmp = std::stoi(number);
	number = std::to_string(tmp);
	return number;
}

long int blink_rec(std::string number, int i, int max, std::unordered_map<std::string, long int>& map) {
	std::string key = number + "-" + std::to_string((max-i));

	if (map.find(key) != map.end()) {
		return map[key];
	}

	long int count = 0;
	std::vector<std::string> new_numbers;
	if (number == "0") {
		new_numbers.push_back("1");
	} else if (number.size() % 2 == 0) {
		std::string s1 = checkZeros(number.substr(0, number.length()/2));
		std::string s2 = checkZeros(number.substr(number.length()/2));
		new_numbers.push_back(s1);
		new_numbers.push_back(s2);
	} else {
		long int tmp = std::stol(number);
		tmp *= 2024;
		new_numbers.push_back(std::to_string(tmp));
	}

	if (i == max) {
		return new_numbers.size();
	}

	for (size_t j = 0; j < new_numbers.size(); j++) {
		count += blink_rec(new_numbers[j], i+1, max, map);
	}
	map[key] = count;
	return count;
}

long int countStones(std::vector<std::string> numbers, int n, std::unordered_map<std::string, long int>& map) {
	long int nstones = 0;

	for (size_t i = 0; i < numbers.size(); i++) {
		nstones += blink_rec(numbers[i], 1, n, map);
	}
	return nstones;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> numbers = readFile("input.txt");
	std::unordered_map<std::string, long int> map;
	//printVector(numbers);

	// Challenge 1
	long int n = countStones(numbers, 25, map);
	std::cout << "There will be " << n << " stones with 25 blinks" << std::endl;

	// Challenge 2
	n = countStones(numbers, 75, map);
	std::cout << "There will be " << n << " stones with 75 blinks" << std::endl;


	return 0;
}