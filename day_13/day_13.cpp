#include <iostream>
#include <fstream>
#include <regex>


long int calcPrice(std::string filename, long int extra) {
	std::fstream in;
	in.open(filename, std::ios::in);
	if (!in) {
		std::cout << "Error on file opening" << std::endl;
		exit(1);
	}

	std::string pattern_string = R"((Button [AB]|Prize): X[+=-](\d+), Y[+=-](\d+))";
	std::regex pattern(pattern_string);

	long int price = 0;
	std::string s;
	while (std::getline(in, s)) {
		if (s != "\n") {
			long int button_A_x = 0;
			long int button_A_y = 0;
			long int button_B_x = 0;
			long int button_B_y = 0;
			long int prize_x = 0;
			long int prize_y = 0;

			std::smatch matchesA;
			std::string::const_iterator searchStart(s.cbegin());
			while (std::regex_search(searchStart, s.cend(), matchesA, pattern)) {
				button_A_x = std::stol(matchesA[2].str());
				button_A_y = std::stol(matchesA[3].str());
				searchStart = matchesA.suffix().first;
			}

			std::getline(in, s);
			std::smatch matchesB;
			std::string::const_iterator searchStart2(s.cbegin());
			while (std::regex_search(searchStart2, s.cend(), matchesB, pattern)) {
				button_B_x = std::stol(matchesB[2].str());
				button_B_y = std::stol(matchesB[3].str());
				searchStart2 = matchesB.suffix().first;
			}

			std::getline(in, s);
			std::smatch matches_prize;
			std::string::const_iterator searchStart3(s.cbegin());
			while (std::regex_search(searchStart3, s.cend(), matches_prize, pattern)) {
				prize_x = std::stol(matches_prize[2].str()) + extra;
				prize_y = std::stol(matches_prize[3].str()) + extra;
				searchStart3 = matches_prize.suffix().first;
			}
			std::getline(in, s);
			// std::cout << button_A_x << " " << button_A_y << " " << button_B_x << " " << button_B_y << " " << prize_x << " " << prize_y << " " << std::endl;
			long int den =  ((button_B_x * button_A_y) - (button_B_y * button_A_x));
			long int n = ((button_A_y * prize_x) - (button_A_x * prize_y));
			long int m = ((button_B_x * prize_y) - (button_B_y * prize_x));
			if (n % den == 0 && m % den == 0) {
				n = n/den;
				m = m/den;
				price += 3*m + n;
			}
		}
	}

	in.close();
	return price;
}

int main(int argc, char* argv[]) {
	// Challenge 1
	long int price = calcPrice("input.txt", 0);
	std::cout << "price is: " << price << std::endl;

	// Challenge 2
	price = calcPrice("input.txt", 10000000000000);
	std::cout << "price with 10000000000000 before : " << price << std::endl;

	return 0;
}