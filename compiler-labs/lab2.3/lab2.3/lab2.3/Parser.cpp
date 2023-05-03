#include "Parser.h"
#include <algorithm>
#include <iostream>

using namespace std;

Parser::Parser(const vector<Tokenizer::Token>& tokens) {
	tkn_it = tkn_it = tokens.begin();
	tkn_end = tokens.end();

	using gr = GrammarItem;
	using tkn = Tokenizer::TokenType;

	rules = {
		{{gr::prog, tkn::ParL}, {gr::left, tkn::Eq, gr::expr, gr::prog}},
		{{gr::prog, tkn::Eof}, {}},
		{{gr::left, tkn::String}, {tkn::ParR}},
		{{gr::left, tkn::Axiom}, {tkn::String, tkn::ParR}},
		{{gr::expr, tkn::String}, {gr::expr}},
		{{gr::expr, tkn::ParL}, {tkn::String, tkn::ParR, gr::expr }},
		{{gr::expr, tkn::Bar}, {gr::expr}},
		{{gr::expr, tkn::Dot}, {}},
	};

	for (auto& rule : rules) {
		reverse(rule.second.begin(), rule.second.end());
	}
}

void Parser::parse() {
	using gr = GrammarItem;
	using tkn = Tokenizer::TokenType;
	//grammar_stack.push_back(gr::end);
	grammar_stack.push_back(gr::prog);
	//vector<> out_stack

	while (grammar_stack.size() && tkn_it != tkn_end) {
		print_stack();

		if (holds_alternative<tkn>(grammar_stack.back()) &&
			get<tkn>(grammar_stack.back()) == tkn_it->type) {
		} else if (holds_alternative<gr>(grammar_stack.back())) {
			auto g = get<gr>(grammar_stack.back());
			if (rules.count({ g, tkn_it->type})) {
				print_rule({ { g, tkn_it->type }, rules[{ g, tkn_it->type }] });
				cout << "\n";
				grammar_stack.pop_back();
				for (auto r : rules[{ g, tkn_it->type }]) {
					grammar_stack.push_back(r);
				}
				grammar_stack.push_back(tkn_it->type);
				print_stack();
			}
			else {
				cout << "Error\n";
				break;
			}			
		}

		grammar_stack.pop_back();
		cout << *tkn_it << "\n";
		tkn_it++;
		if (tkn_it == tkn_end && !grammar_stack.size()) {
			cout << "Success!\n";
			//grammar_stack.pop_back();
			break;
		}
	}
}

void Parser::print_stack() {
	cout << "On stack: ";
	for (auto s = grammar_stack.rbegin(); s != grammar_stack.rend(); s++) {
		if (holds_alternative<GrammarItem>(*s))
			cout << get<GrammarItem>(*s);
		if (holds_alternative<Tokenizer::TokenType>(*s))
			cout << get<Tokenizer::TokenType>(*s);
		cout << " ";
	}
	cout << "\n";
}

ostream& operator<<(ostream& os, const Parser::GrammarItem& gri) {
	map<Parser::GrammarItem, string> grnames = {
		{Parser::GrammarItem::prog, "prog"},
		{Parser::GrammarItem::left, "left"},
		{Parser::GrammarItem::expr, "expr"},
		{Parser::GrammarItem::symbol, "symbol"},
		{Parser::GrammarItem::end, "end"},
	};
	os << grnames[gri];
	return os;
}

void Parser::print_rule(const pair<pair<GrammarItem, Tokenizer::TokenType>, vector<symbol>>& rule) {
	cout << rule.first.first << " -> ";
	cout << rule.first.second << " ";
	for (auto s = rule.second.rbegin(); s != rule.second.rend(); s++) {
		if (holds_alternative<GrammarItem>(*s))
			cout << get<GrammarItem>(*s);
		if (holds_alternative<Tokenizer::TokenType>(*s))
			cout << get<Tokenizer::TokenType>(*s);
		cout << " ";
	}
}
