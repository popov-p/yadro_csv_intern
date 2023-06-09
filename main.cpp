#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "reader.h"

using namespace std;

int main(int argc, char* argv[])
{
	if (argc < 2) {
		cout << "usage: csv_intern.exe <filename> (must be inside the same directory as .exe)" << endl;
		return 1;
	}
	string filename = argv[1];
	try {
		Reader rdr = Reader("example.csv");
		rdr.start(cin, cout);
	}
	catch (exception& e) {
		cout << e.what()<<endl;
	}
	return 0;
}
