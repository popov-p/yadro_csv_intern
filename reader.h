#pragma once

#include <fstream>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <set>
#include <functional>

class Reader {
public:
	Reader() = delete;
	Reader(const Reader&) = delete;
	Reader& operator= (const Reader&) = delete;
	Reader(const std::string& filename);
	void start(std::istream& input, std::ostream& output);
	~Reader() = default;
private:
	void open_filestream();
	void set_display_mode(std::istream& input);
	void print_raw(std::ostream& output);
	void print_table(std::ostream& output);
	void load_data();
	void expression_validator(std::string& val);
	void validator(std::vector<std::string>& cells);
	void operand_validator(std::string& operand);
	std::string get_cell_value(std::string& row, int& col);
	void update_cell_value(std::string& row, int col, std::string& element);
	void injection();
	void evaluate_expressions();
	void count_value(std::string expr, std::pair<std::string, int> buffer);
	std::pair<int, std::string> split_operand(std::string& operand);

	std::string _filename;
	std::ifstream _file;
	bool _evaluate_expressions = false;
	std::vector<std::string> _not_associated_headers;
	std::map <std::string, int> _headers;
	std::map<std::string, std::vector<std::string>> _table;
	std::list<std::pair<std::string, int>> _expressions; /*expression coordinates in table*/
	std::set<std::pair<std::string, int>> _calculation_stack;
	std::map<char, std::function<int(int, int)>> _operations;

};
