#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "reader.h"

using namespace std;

int main()
{
	Reader rdr = Reader("qwe.txt");
	rdr.set_display_mode(std::cin);
	//cout << "Hello CMake." << endl;
	return 0;
}
