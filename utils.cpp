#include "utils.h"
#include <stdexcept>
#include <iostream>
#include <sstream>

using namespace std;

bool startswith(std::string& str, const string& prefix) {
	if (prefix.length() > str.length()) {
		return false;
	}
	return str.compare(0, prefix.length(), prefix) == 0;
}

void split_row(const std::string row, vector<std::string>& cells, char delimeter) {
	string cell;
	istringstream iss(row);
	while (std::getline(iss, cell, delimeter)) {
		cells.push_back(cell);
	}
}

void print_tokenized_row(std::ostream& output, std::vector<std::string> to_print) {
	int  counter = 0;
	if (to_print.empty()) {
		throw invalid_argument("trying to print an empty vector (tokenized row)");
	}
	else {
		for (counter; counter < to_print.size() - 1; counter++) {
			output << to_print[counter] << ", ";
		}
		output << to_print[counter] << endl;
	}
}