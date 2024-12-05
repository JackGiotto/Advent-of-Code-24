#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h> // std::sort

// CHALLENGE 1
void readFile(const char filename[], std::vector<int>& first_column, std::vector<int>& second_column) {
	std::fstream in;
	in.open(filename, std::ios::in);
	if (!in.is_open()) {
		std::cerr << "Error: Could not open file " << filename << std::endl;
		exit(1);
	} else {
		int first, second;
		while (in >> first >> second) {
			first_column.push_back(first);
			second_column.push_back(second);
		}
	}
	in.close();
}

int countDistance(std::vector<int>& first_column, std::vector<int>& second_column) {
	int distance = 0;

	for (size_t i = 0; i < first_column.size(); i++) {
		distance += std::abs(first_column[i] - second_column[i]);
	}
	return distance;
}

// CHALLENGE 2

int findSimilarityScore(const std::vector<int> first_column, const std::vector<int> second_column) {
	int similarity_score = 0;
	size_t last_index = 0;
	for (size_t i = 0; i < first_column.size(); i++) {
		int count = 0;
		size_t j = last_index;
		while (second_column[j] <= first_column[i] && j < second_column.size()) {
			if (first_column[i] == second_column[j]) {
				count++;
			}
			j++;
		}
		last_index = j;
		similarity_score += first_column[i] * count;
	}
	return similarity_score;
}

int main(int argc, char* argv[]) {
	// CHALLENGE 1
	std::vector<int> first_column;
	std::vector<int> second_column;
	char filename[] = "input.txt";

	readFile(filename, first_column, second_column);
	std::sort(first_column.begin(), first_column.end());
	std::sort(second_column.begin(), second_column.end());

	int distance = countDistance(first_column, second_column);
	std::cout << distance << std::endl;
	// 2756096

	// CHALLENGE 2

	int similarity_score = findSimilarityScore(first_column, second_column);
	std::cout << similarity_score << std::endl;
	// 23117829

	return 0;
}