#pragma once
#include <string>
#include <optional>
#include <vector>
#include <iterator>

using namespace std;

/*******************************************************************************
TODO: put the description here
*******************************************************************************/
class Tokenizer {
public:
	enum class TokenType {
		ParL, // (
		ParR, // )
		Bar, // |
		Dot, // .
		Eq, // =
		Axiom, // axiom keyword
		String, // any string
		Eof, // end of file
		Err // Error token
	};

	struct Token {
		TokenType type = TokenType::Err;
		string value = "";
		size_t pos = 0;
		Token() {};
		Token(TokenType type, string value, size_t pos) :
			type(type), value(value), pos(pos) {};
		friend ostream& operator<<(ostream& os, const Token& tkn);
	};

	friend ostream& operator<<(ostream& os, const TokenType& tkn);

	Tokenizer(const string& str);

	// Reads token starting from pos, modifies pos 
	optional<Token> read_token();

	// Returns vector of all tokens
	vector<Token> tokenize();

private:
	// String to be parsed
	string::const_iterator str_begin;
	string::const_iterator str_end;
	string::const_iterator str_it;

	// Returns current position
	size_t pos();

	// Checks if the symbol is space symbol
	bool space_symbol(char c);

	//  Isend reached
	bool end_encountered = false;

	// Moves pos futher though space symbols & comments
	void skip();
};

