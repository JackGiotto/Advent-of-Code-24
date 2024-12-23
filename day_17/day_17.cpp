#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <cmath>
#include <climits>

struct Computer {
	long long registers[3];
	std::vector<int> instructions;

	Computer(long long a, long long b, long long c, const std::vector<int>& t_istructions) {
		registers[0] = a;
		registers[1] = b;
		registers[2] = c;
		instructions = t_istructions;
	}

	long long findCorrectA() {
		long long a = 0;
		long long mult_8 = 8;
		for (size_t i = 0; i < instructions.size()-1; i++) {
			for (size_t j = 0; j < i; j++) {
				mult_8 *= 8;
				if (mult_8 > LLONG_MAX / 8) {
					std::cout << "Overflow detected, stopping calculation." << std::endl;
					return a;
				}
			}
			a += mult_8 * (long long)instructions[i];
			if (a < 0) {
				std::cout << "Overflow detected, stopping calculation." << std::endl;
				return a;
			}
			std::cout << a << std::endl;
			mult_8 = 8;
		}
		std::cout << a << std::endl;
		return a;
	}

	std::vector<int> execute() {
		int ptr = 0;
		std::vector <int> output;

		// std::cout << "a: " << registers[0] << ", b: " << registers[1] << ", c: " << registers[2] << std::endl;
		std::cout << "instructions: ";
		for (const auto& instr : instructions) {
			std::cout << instr << " ";
		}
		std::cout << std::endl;

		while (ptr < instructions.size()) {
			switch (instructions[ptr]) {
			case 0:
				adv(instructions[ptr+1]);
				ptr += 2;
				break;
			case 1:
				bxl(instructions[ptr+1]);
				ptr += 2;
				break;
			case 2:
				bst(instructions[ptr+1]);
				ptr += 2;
				break;
			case 3:
				jnz(instructions[ptr+1], ptr);
				break;
			case 4:
				bxc(instructions[ptr+1]);
				ptr += 2;
				break;
			case 5:
				out(instructions[ptr+1], output);
				ptr += 2;
				break;
			case 6:
				bdv(instructions[ptr+1]);
				ptr += 2;
				break;
			case 7:
				cdv(instructions[ptr+1]);
				ptr += 2;
				break;
			default:
				break;
			}
		}
		return output;
	}

	int combo(const int operand) {
		if (operand >= 0 && operand <= 3) return operand;
        if (operand == 4) return registers[0];
        if (operand == 5) return registers[1];
        if (operand == 6) return registers[2];
		
		return -1;
	}

	void adv(const int operand) {
		registers[0] = registers[0] / (int)(std::pow(2, combo(operand)));
	}

    void bxl(const int operand) {
        registers[1] = registers[1] ^ operand;
    }

    void bst(const int operand) {
        registers[1] = combo(operand) % 8;
    }

    void jnz(const int operand, int& instr_ptr) {
        if (registers[0] != 0) instr_ptr = operand;
        else instr_ptr += 2;
    }

    void bxc(const int operand) {
        registers[1] = registers[1] ^ registers[2];
    }

    void out(const int operand, std::vector<int>& output) {
        const auto result = std::to_string(combo(operand) % 8);
        for (const auto ele : result) {
            output.push_back(ele - '0');
        }
    }

	void bdv(const int operand) {
        registers[1] = registers[0] / std::pow(2, combo(operand));
    }

    void cdv(const int operand) {
        registers[2] = registers[0] / std::pow(2, combo(operand));
    }
};

Computer readFile(std::string filename, long long hardcoded = 0) {
	std::fstream file;
	file.open(filename, std::ios::in);
	if (!file) {
		std::cout << "Error on matrix file opening" << std::endl;
		exit(1);
	}

	std::string line;

	std::getline(file, line);
	const long long a = (hardcoded == 0) ? std::stoll(line.substr(12, line.size() - 12)) : hardcoded;

	std::getline(file, line);
	const long long b = std::stoll(line.substr(12, line.size() - 12));

	std::getline(file, line);
    const long long c = std::stoll(line.substr(12, line.size() - 12));

	std::vector<int> instructions;
	std::getline(file, line);
	std::getline(file, line, ' ');

	while(std::getline(file, line, ',')) {
		//std::cout << line << std::endl;
		instructions.push_back(std::stoi(line));
	}

	std::cout << "a: " << a << ", b: " << b << ", c: " << c << std::endl;

	file.close();
	Computer comp = Computer(a, b, c, instructions);
	return comp;
}

int main(int argc, char* argv[]) {
	Computer c = readFile("input.txt");

	std::vector<int> result = c.execute();

	for (int i = 0; i < result.size() - 1; i++) {
		std::cout << result[i] << ',';
	}

	std::cout << result[result.size()-1] << std::endl;

	//c.findCorrectA();

	return 0;
}