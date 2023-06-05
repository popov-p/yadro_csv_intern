#include "reader.h"
#include <iostream>

using namespace std;

Reader::Reader(const string & filename) {
	if (filename.empty()) {
		throw invalid_argument("Empty filename");
	}
	_file = ifstream(filename);
};

void Reader::start() {

}
void Reader::set_display_mode(std::istream& input) {
	string msg = "";
	cout << "evaluate expressions [y/n] ?" << endl;
	while (cin >> msg) {
		if (msg == "y") {
			_evaluate_expressions = true;
			cout << "all expressions will be evaluated" << endl;
			return;
		}
		else if (msg == "n") {
			//_evaluate_expressions = false;
			cout << "no expressions will be evaluated" << endl;
			return;
		}
		else {
			cout << "evaluate expressions [y/n] ?" << endl;
		}
	}
};