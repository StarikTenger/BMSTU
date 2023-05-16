#pragma once
#include "Tokenizer.h"
#include "SyntaxTree.h"
#include <variant>
#include <vector>
#include <map>

using namespace std;

class Parser {
public:
	Parser(const vector<Tokenizer::Token>& tokens);
	shared_ptr<NonTerm> parse();

private:
	vector<Tokenizer::Token>::const_iterator tkn_it;
	vector<Tokenizer::Token>::const_iterator tkn_begin;
	vector<Tokenizer::Token>::const_iterator tkn_end;

	using symbol = variant<GrammarItem, Tokenizer::TokenType>;

	// Info aabout current symbol and corresponding node
	vector<pair<symbol, shared_ptr<NonTerm>>> grammar_stack;

	map<pair<GrammarItem, Tokenizer::TokenType>, vector<symbol>> rules;

	void print_rule(const pair<pair<GrammarItem, Tokenizer::TokenType>, vector<symbol>>& rule);

	void print_stack();
};