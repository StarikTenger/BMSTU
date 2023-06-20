#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>

using namespace std;

class Scanner {
public:
	struct Position {
		size_t line = 0;
		size_t pos = 0;
		string::const_iterator str_it;
		Position() {}
		Position(size_t line, size_t pos) : line(line), pos(pos) {}
		Position operator++(int);
	};

	struct Fragment {
		Position start;
		Position end;
		Fragment() {};
		Fragment(Position start, Position end) : start(start), end(end) {}
	};

	struct Message {
		bool error;
		string text;
		Fragment coord;
	};

	enum DomainTag {
		Str, 
		Num,
		Eof
	};

	struct Token {
		DomainTag tag;
		Fragment coord;
		void print();
	protected:
		virtual void print_attr() = 0;
	};

	struct TokenStr : public Token {
		string attr;
	protected:
		void print_attr() override;
	};
	
	struct TokenNum : public Token {
		long long attr;
	protected:
		void print_attr() override;
	};

	Scanner(const string& str);

	Scanner::Token* next_token();

private:
	string::const_iterator str_begin;
	string::const_iterator str_end;
	Position cur_pos;

	void report_error(string message);
	TokenStr* scan_str();
	TokenNum* scan_num();
	bool is_digit(char c);
	bool space_symbol(char c);
	bool reached_end();
	void skip();
};