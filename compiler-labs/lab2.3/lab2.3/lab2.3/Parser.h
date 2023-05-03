#pragma once
#include "Tokenizer.h"
#include <variant>
#include <vector>
#include <map>

using namespace std;

class Parser {
public:
	Parser(const vector<Tokenizer::Token>& tokens);
	void parse();

private:
	vector<Tokenizer::Token>::const_iterator tkn_it;
	vector<Tokenizer::Token>::const_iterator tkn_begin;
	vector<Tokenizer::Token>::const_iterator tkn_end;

	enum class GrammarItem {
		prog, left, expr, symbol, end
	};

	friend ostream& operator<<(ostream& os, const GrammarItem& gri);

	using symbol = variant<GrammarItem, Tokenizer::TokenType>;

	vector<symbol> grammar_stack;

	map<pair<GrammarItem, Tokenizer::TokenType>, vector<symbol>> rules;

	void print_rule(const pair<pair<GrammarItem, Tokenizer::TokenType>, vector<symbol>>& rule);

	void print_stack();
};