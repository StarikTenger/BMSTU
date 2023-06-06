%{
	/* need this for the call to atof() below */
	#include <math.h>
	#include <iostream>
	#include <string>
	#include <map>
	size_t num_chars = 0;
	size_t cur_id = 0;
	std::map<std::string, int> id_table;

	#define YY_USER_ACTION num_chars += yyleng;
	void printtok(std::string str) {
		std::cout << "[" << str << " " << num_chars-yyleng << "-" << num_chars  << "]: " << yytext << "\n";
	}
%}

%option noyywrap

DIGIT [0-9]
ID [A-Z][A-Za-z0-9\-]*
DIR \$[A-Z]+

%%

{ID} {
	printtok("ID");
	if (!id_table[yytext]) {
		id_table[yytext] = ++cur_id;
	}
	std::cout << id_table[yytext] << "\n";
}
{DIR} printtok("DIR");

"{"[\^{}}\n]*"}" {}/* eat up one-line comments */

[" "\t\n]+ {}/* eat up whitespace */

%%

int main( int argc, char **argv ) {
	++argv, --argc; /* skip over program name */
	yylex();
}