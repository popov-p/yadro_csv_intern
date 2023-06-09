#pragma once

#include <string>
#include <vector>

bool startswith(std::string& str, const std::string& prefix);
void split_row(const std::string row, std::vector<std::string>& cells, char delimeter);
void print_tokenized_row(std::ostream& output, std::vector<std::string> to_print);