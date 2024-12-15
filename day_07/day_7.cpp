#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

struct Row {
	long int result;
	std::vector<long int> values;
};

void printVector(const std::vector<struct Row>& rows) {
	for (const auto& row : rows) {
		std::cout << "Result: " << row.result << " Values: ";
		for (const auto& value : row.values) {
			std::cout << value << " ";
		}
		std::cout << std::endl;
	}
}

bool checkRowRec(const std::vector<long int> &numbers, size_t index, long int currentResult, long int& result, bool concat) {
	if (currentResult > result) {
	  return false;
	}

	if (index == numbers.size() && currentResult == result) {
	  return true;
	}

	if (index == numbers.size()) {
	  return false;
	}

	bool a = checkRowRec(numbers, index + 1, currentResult + numbers[index], result, concat);
	bool b = checkRowRec(numbers, index + 1, currentResult * numbers[index], result, concat);
	bool c = concat ?
	checkRowRec(numbers, index + 1,
		std::stoll(std::to_string(currentResult) + std::to_string(numbers[index])),
		result, true
	)
	: false;
	return a || b || c;
}

long int sumResults(std::vector<Row> rows, bool concat = false) {
	long int sum = 0;

	for (size_t i = 0; i < rows.size(); i++) {
		if (checkRowRec(rows[i].values, 1, rows[i].values[0], rows[i].result, concat)) {
			sum += rows[i].result;
			//sum++;
		}
	}
	return sum;
}

std::vector<struct Row> readFile(std::string filename) {
	std::fstream in;
	in.open(filename, std::ios::in);

	std::string line;
	std::vector<struct Row> rows;
	while (std::getline(in, line)) {
		std::stringstream numbers_row(line);
		std::string number_string;
		std::getline(numbers_row, number_string, ':');
		struct Row r;

		long int result = std::stol(number_string.c_str());
		r.result = result;
		std::vector<long int> values;
		std::getline(numbers_row, number_string, ' ');
		while (std::getline(numbers_row, number_string, ' ')) {
			values.push_back(std::atoi(number_string.c_str()));
		}
		r.values = values;
		rows.push_back(r);
	}

	in.close();
	return rows;
}

int main(int argc, char* argv[]) {
	// Challenge 1
	std::vector<struct Row> rows = readFile("input.txt");
	long int sum = sumResults(rows);

	std::cout << "Sum: " << sum << std::endl;

	// Challenge 2
	sum = sumResults(rows, true);
	std::cout << "Sum with concat: " << sum << std::endl;

	return 0;
}