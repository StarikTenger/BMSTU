#ifndef DRIVER_HH
# define DRIVER_HH
# include <string>
# include <map>
# include "parser.hpp"

// Give Flex the prototype of yylex we want ...
# define YY_DECL \
  yy::parser::symbol_type yylex (driver& drv)
// ... and declare it for the parser's sake.
YY_DECL;

// Conducting the whole scanning and parsing of Calc++.
class driver {
public:
	driver ();

	std::map<std::string, int> variables;

	int result;

	size_t nesting = 0;
	size_t col_limit = 80;

	// Run the parser on file F.  Return 0 on success.
	int parse (const std::string& f);
	// The name of the file being parsed.
	std::string file;
	// Whether to generate parser debug traces.
	bool trace_parsing;

	// Handling the scanner.
	void scan_begin ();
	void scan_end ();
	// Whether to generate scanner debug traces.
	bool trace_scanning;
	// The token's location used by the scanner.
	yy::location location;

	void print(std::string str) {
		cur_col += str.size();
		if (cur_col > col_limit) {
			newline();
			print_tab();
		}
		std::cout << str;
	}

	void newline() {
		cur_col = 0;
		std::cout << "\n";
	}

	void print_tab() {
		cur_col += nesting * 4;
		for (size_t i=0; i<nesting; i++) printf("\t");
	}
private:
	size_t cur_col = 0;
};
#endif // ! DRIVER_HH