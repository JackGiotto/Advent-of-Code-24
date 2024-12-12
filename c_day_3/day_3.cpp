#include <iostream>
#include <fstream>
#include <regex>
#include <string>

int sumProducts(std::string filename, bool check_do = false) {
	std::fstream in;
	in.open(filename, std::ios::in);
	if (!in) {
		std::cout << "Error on file opening" << std::endl;
		exit(1);
	}

	std::string pattern_regex = check_do ?
		R"(mul\((\d+),(\d+)\)|(do\(\))|(don't\(\)))" :
		R"(mul\((\d+),(\d+)\))";

	std::regex pattern(pattern_regex);
	std::string line;
	int sum = 0;

	bool flag = true;
	while (std::getline(in, line)) {
		std::smatch matches;
		std::string::const_iterator searchStart(line.cbegin());
		while (std::regex_search(searchStart, line.cend(), matches, pattern)) {
			if (matches[0] == "do()") {
				flag = true;
			} else if (matches[0] == "don't()") {
				flag = false;
			} else if (flag) {
				int n1 = std::stoi(matches[1].str());
				int n2 = std::stoi(matches[2].str());
				sum += n1 * n2;
			}
			searchStart = matches.suffix().first;
		}
	}

	in.close();
	return sum;
}

int main(int argc, char* argv[]) {
	// Challenge 1
	int sum = sumProducts("input.txt");
	std::cout << "sum: " << sum << std::endl;

	// Challenge 2
	sum = sumProducts("input.txt", true);
	std::cout << "sum (with [do()] an [don't()]): " << sum << std::endl;

	return 0;
}