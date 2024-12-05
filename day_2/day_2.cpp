#include <iostream>
#include <fstream>
#include <sstream>

struct node {
	int value;
	struct node *next;
};

struct dmatrix {
	struct node* current_column;
	struct dmatrix* next_column;
};

node* addElement(node* head, int data) {
	node* newNode = new node {data, nullptr};
	if (head == nullptr) {
		return newNode;
	} else {
		node* h2 = head;
		while (h2 -> next != nullptr) {
			h2 = h2 -> next;
		}
		h2 -> next = newNode;
	}
	return head;
}

dmatrix* readFile(const char filename[]) {
	struct dmatrix* matrix = new dmatrix {nullptr, nullptr};
	std::fstream in;
	in.open(filename, std::ios::in);

	struct dmatrix* matrix2 = matrix;

	if (!in) {
		std::cout << "Error on file opening" << std::endl;
		exit(1);
	} else {
		std::string line;
		int number = 0;
		while (std::getline(in, line)) {
			node *head = matrix2 -> current_column;
			std::stringstream ss(line);
			while (ss >> number) {
				head = addElement(head, number);
			}
			matrix2 -> current_column = head;
			dmatrix* temp = new dmatrix;
			temp -> current_column = nullptr;
			temp -> next_column = nullptr;
			matrix2 -> next_column = temp;
			matrix2 = temp;
		}
	}
	in.close();
	return matrix;
}

bool isSafeWithIgnore(node* head, int ignore_index = -1) {
	if (head == nullptr) return false;
	int i = 0;
	int ff = 0;
	int previous_number;
	if (i == ignore_index) {
		head = head->next;
		if (head == nullptr) return false;
		previous_number = head -> value;
		head = head -> next;
		i++;
	} else {
		previous_number = head->value;
		head = head -> next;
		i++;
	}
	while (head != nullptr) {
		if (i != ignore_index) {
			int difference = head -> value - previous_number;
			if (ff == 0) {
				ff = (difference > 0) ? 1 : -1;
			}
			difference *= ff;
			if (!(difference <= 3 && difference > 0)) {
				return false;
			}
			previous_number = head -> value;
			head = head -> next;
		} else {
			head = head -> next;
		}
		i++;
	}
	return true;
}

bool isSafe(node* head, bool ignore = false) {
	if (head == nullptr) return false;
	if (ignore) {
		int i = -1;
		node *h2 = head;
		for (i = -1; h2 != nullptr; i++, h2 = h2 -> next) {
			if (isSafeWithIgnore(head, i)) {
				return true;
			}
		}
		if (isSafeWithIgnore(head, i)) {
				return true;
		}
	return false;
	} else {
		return isSafeWithIgnore(head);
	}
}

int countSafe(dmatrix* matrix, bool ignore = false) {
	int count = 0;
	while (matrix != nullptr) {
		if (isSafe(matrix -> current_column, ignore)) {
			count++;
		}
		matrix = matrix -> next_column;
	}
	return count;
}

int main(int argc, char* argv[]) {
	char filename[] = "input.txt";
	struct dmatrix* matrix = new dmatrix;
	matrix -> current_column = new node;
	matrix -> next_column = nullptr;

	matrix = readFile(filename);

	// Challenge 1
	int count = countSafe(matrix);
	std::cout << count << " righe sicure" << std::endl;

	// Challenge 2
	count = countSafe(matrix, true);
	std::cout << count << " righe sicure ignorando al massimo un elemento per riga" << std::endl;

	return 0;
}