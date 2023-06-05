#include <iostream>
#include <stdlib.h>
#include <sstream>
#include "driver.hpp"

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

int main (int argc, char *argv[]) {
	int res = 0;
	driver drv;
	if (argc <= 1) {
		std::cout << "ERROR: expected a file name\n";
	}
	for (int i = 1; i < argc; ++i) {
		if (argv[i] == std::string ("-h")) {
				std::cout << "USAGE: specify a filename as an argument\n\t-h help\n\t-l specify column limit\n";
		} else if (argv[i] == std::string ("-l")) {
			if (argc > i + 2 && is_number(std::string(argv[i + 1])) && atoi(argv[i + 1]) > 0) {
				drv.col_limit = atoi(argv[i + 1]);
				i++;
			}
			else
				std::cout << "ERROR: expected an unsigned int after -l\n";
		}
		else if (!drv.parse (argv[i]))
			std::cerr << drv.result << '\n';
		else {
			std::cout << "ERROR: expected a file name\n";
		}
	}
	return res;
}
