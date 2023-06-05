#ifndef READER_H
#define READER_H

#include <fstream>
#include <string>
class Reader {
public:				
	Reader(const std::string& filename);
	void set_display_mode(std::istream& input);
private:
	std::ifstream _file;
	bool _evaluate_expressions = false;
};

#endif