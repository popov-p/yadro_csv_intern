#include "reader.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

Reader::Reader(const string& filename) {
	if (filename.empty()) {
		throw invalid_argument("Empty filename");
	}
	_filename = filename;
	if (!_file) {
		throw runtime_error("error to create input filestream");
	}
	_operations['+'] = [](int op1, int op2) {return op1 + op2; };
	_operations['-'] = [](int op1, int op2) {return op1 - op2; };
	_operations['*'] = [](int op1, int op2) {return op1 * op2; };
	_operations['/'] = [](int op1, int op2) {
		if (op2 == 0) {
			throw runtime_error("division by zero");
		}
		return op1 / op2;
	};
};

void Reader::open_filestream() {
	_file.open(_filename);
	if (!_file.is_open()) {
		throw runtime_error("file was not open: " + _filename);
	}
};

void Reader::injection() {
	for (int i = 1; i < _not_associated_headers.size(); i++) {
		_headers[_not_associated_headers[i]] = i - 1;
	}
};

void Reader::validator(vector<string>& cells) {
	int check = stoi(cells[0]);
	if (cells.size() > _not_associated_headers.size()) {
		throw runtime_error("error while parsing a string: too low number of headers");
	}
	for (int i = 1; i < cells.size(); i++) {
		if (!cells[i].empty()) {
			if (!startswith(cells[i], "=")) {
				check = stoi(cells[i]);
			}
			else {
				expression_validator(cells[i]);
				if (_evaluate_expressions) {
					_expressions.push_back({ cells[0], i - 1 });
				}
			}
		}
	}
	if (cells.size() < _headers.size()) {
		for (int i = 0; i <= (_not_associated_headers.size() - cells.size()); i++) {
			cells.push_back("");
		}
	}
};

void Reader::expression_validator(string& cell) {
	if (startswith(cell, "=")) {
		string expression = cell.substr(1);
		pair <string, string> operands;
		size_t oper_pos = expression.find_first_of("+-*/");
		if (oper_pos == string::npos) {
			throw runtime_error("expression does not contain any of +/*- operators");
		}
		operands.first = expression.substr(0, oper_pos);
		operand_validator(operands.first);
		expression = expression.substr(oper_pos + 1);
		oper_pos = expression.find_first_of("+-*/");
		if (oper_pos != string::npos) {
			throw runtime_error("expression contains more than one operation sign");
		}
		operands.second = expression;
		operand_validator(operands.second);
	}
	else {
		throw runtime_error("expression must start with =");
	}
};

void Reader::operand_validator(string& operand) {
	if (operand.empty()) {
		throw runtime_error("one of operands is empty");
	}
	size_t i = 0;
	while (i < operand.size() && isalpha(operand[i])) {
		i++;
	}
	if (i > 0 && i < operand.size() && isdigit(operand[i])) {
		i++;
		while (i < operand.size() && isdigit(operand[i])) {
			i++;
		}
		if (i != operand.size()) {
			throw runtime_error("one of operands has wrong operand format");
		}
	}
	else {
		throw runtime_error("one of operands has wrong operand format (no digits)");
	}
};

void Reader::load_data() {
	string row;
	string raw_headers;
	open_filestream();
	getline(_file, raw_headers);
	if (!startswith(raw_headers, ",")) {
		throw runtime_error("left table corner must be a comma");
	}
	split_row(raw_headers, _not_associated_headers, ',');
	injection();
	while (getline(_file, row)) {
		vector<string> cells;
		split_row(row, cells, ',');
		validator(cells);
		string key = cells[0];
		cells.erase(cells.begin());
		_table[key] = cells;
		row.clear();
		cells.clear();
		key.clear();
	}
};

string Reader::get_cell_value(string& row, int& col) {
	auto it = _table.find(row);
	if (it != _table.end()) {
		if (col < _table[row].size()) {
			return _table[row][col];
		}
		else {
			throw runtime_error("trying to vector peek element outside borders");
		}
	}
	else {
		throw runtime_error("trying to peek table element outside borders");
	}
};

void Reader::update_cell_value(string& row, int col, string& element) {
	if (col < _headers.size()) {
		_table[row][col] = element;
	}
	else {
		throw runtime_error("column index is out of range");
	}
};

void Reader::evaluate_expressions() {
	if (_expressions.empty()) {
		return;
	}
	while (!_expressions.empty()) {
		auto expression = _expressions.front();
		string expr = get_cell_value(expression.first, expression.second);
		_calculation_stack.insert(expression);
		_expressions.pop_front();
		count_value(expr, expression);
		if (_expressions.empty()) {
			return;
		}
	}
}
/*col, row*/pair<int, string> Reader::split_operand(string& operand) {
	size_t text_pos = 0;
	while (text_pos < operand.length() && isalpha(operand[text_pos])) {
		text_pos++;
	}
	if (_headers.find(operand.substr(0, text_pos)) == _headers.end()) {
		throw runtime_error("trying to peek an non-existent header");
	}
	else {
		return { _headers[operand.substr(0,text_pos)], operand.substr(text_pos) };
	}
};
void Reader::count_value(string expr, pair<string, int> buffer) {
	string expression = expr.substr(1);
	pair <string, string> operands;
	size_t oper_pos = expression.find_first_of("+-*/");
	char sign = expression[oper_pos];
	operands.first = expression.substr(0, oper_pos);
	expression = expression.substr(oper_pos + 1);
	operands.second = expression;
	pair<int, string> first_col_row = split_operand(operands.first);
	pair<int, string> second_col_row = split_operand(operands.second);
	string op1 = get_cell_value(first_col_row.second, first_col_row.first);
	if (startswith(op1, "=")) {
		pair<string, int> first_buffer = { first_col_row.second, first_col_row.first };
		if (_calculation_stack.find(first_buffer) != _calculation_stack.end()) {
			throw runtime_error("infinite loop");
		}
		_calculation_stack.insert({ first_col_row.second,first_col_row.first });
		count_value(op1, first_buffer);
	}
	string op2 = get_cell_value(second_col_row.second, second_col_row.first);
	if (startswith(op2, "=")) {
		pair<string, int> second_buffer = { second_col_row.second, second_col_row.first };
		if (_calculation_stack.find(second_buffer) != _calculation_stack.end()) {
			throw runtime_error("infinite loop");
		}
		_calculation_stack.insert({ second_col_row.second,second_col_row.first });
		count_value(op2, second_buffer);
	}
	if (_operations.find(sign) != _operations.end()) {
		op1 = get_cell_value(first_col_row.second, first_col_row.first);
		op2 = get_cell_value(second_col_row.second, second_col_row.first);
		string res = to_string(_operations[sign](stoi(op1), stoi(op2)));
		update_cell_value(buffer.first, buffer.second, res);
		auto it = find(_expressions.begin(), _expressions.end(), buffer);
		auto it_calc = find(_calculation_stack.begin(), _calculation_stack.end(), buffer);
		if (it != _expressions.end()) {
			_expressions.erase(it);
		}
		if (it_calc != _calculation_stack.end()) {
			_calculation_stack.erase(it_calc);
		}
	}
	else {
		throw runtime_error("no such operation");
	}
};

void Reader::print_raw(ostream& output) {
	string row;
	string raw_headers;
	open_filestream();
	getline(_file, raw_headers);
	if (!startswith(raw_headers, ",")) {
		throw runtime_error("left table corner must be a comma");
	}
	split_row(raw_headers, _not_associated_headers, ',');
	print_tokenized_row(output, _not_associated_headers);
	while (getline(_file, row)) {
		vector<string> cells;
		split_row(row, cells, ',');
		validator(cells);
		print_tokenized_row(output, cells);
		row.clear();
		cells.clear();
	}
};

void Reader::print_table(ostream& output) {
	print_tokenized_row(output, _not_associated_headers);
	for (auto row : _table) {
		output << row.first << " ";
		print_tokenized_row(output, row.second);
	}
};

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
			cout << "no expressions will be evaluated" << endl;
			return;
		}
		else {
			cout << "evaluate expressions [y/n] ?" << endl;
		}
	}
};

void Reader::start(istream& input, ostream& output) {
	this->set_display_mode(input);
	if (_evaluate_expressions) {
		load_data();
		evaluate_expressions();
		print_table(output);
	}
	else {
		print_raw(output);
	}
};
