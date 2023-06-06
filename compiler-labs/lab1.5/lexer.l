%{
	/* need this for the call to atof() below */
	#include <math.h>
	#include <iostream>
	#include <string>
	#include <map>
	#include <variant>
	size_t cur_line = 1;
	size_t cur_column = 0;
	size_t cur_id = 0;
	using namespace std;

	map<string, size_t> id_table;

	#define YY_USER_ACTION cur_column += yyleng;

	struct Token {
		enum Type {
			ID, DIR
		};
		Type type;

		variant<size_t, string> attr;

		size_t start_line = 0;
		size_t start_column = 0;
		size_t end_line = 0;
		size_t end_column = 0;

		Token(Type type, size_t start_line, size_t start_column,
			size_t end_line, size_t end_column,
			variant<size_t, string> attr) 
			: start_line(start_line), start_column(start_column),
			end_line(end_line), end_column(end_column),
			type(type), attr(attr) {}
		
		friend ostream& operator<<(ostream& os, const Token& tkn);
	};

	ostream& operator<<(ostream& os, const Token& tkn) {
		os << (tkn.type == Token::ID ? "ID" : "DIR") << " ";
		os << "(" << tkn.start_line << ", " << tkn.start_column << ")-";
		os << "(" << tkn.end_line << ", " << tkn.end_column << "): ";
		if (auto attr = get_if<size_t>(&tkn.attr)) os << *attr;
		if (auto attr = get_if<string>(&tkn.attr)) os << *attr;
		cout << "\n";
		return os;
	}
%}

%option noyywrap

DIGIT [0-9]
ID [A-Z][A-Za-z0-9\-]*
DIR \$[A-Z]+

%%

{ID} {
	if (!id_table[yytext]) {
		id_table[yytext] = ++cur_id;
	}
	cout << Token(Token::ID, cur_line, cur_column-yyleng, cur_line, cur_column, id_table[yytext]);
}
{DIR} {
	cout << Token(Token::DIR, cur_line, cur_column-yyleng, cur_line, cur_column, string(yytext));
}

\n {cur_line++; cur_column = 0;}

[" "\t]+ {}/* eat up whitespace */

%%

int main( int argc, char **argv ) {
	++argv, --argc; /* skip over program name */
	yylex();
}