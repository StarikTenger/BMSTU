#include "Parser.h"
#include <algorithm>
#include <iostream>
#include <memory>

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

shared_ptr<NonTerm> Parser::parse() {
	// Type synonyms for easier referencing
	using gr = GrammarItem;
	using tkn = Tokenizer::TokenType;

	// Init syntax tree
	NonTerm root = NonTerm(gr::prog);
	shared_ptr<NonTerm> node_current = shared_ptr<NonTerm>(&root);

	// Put initial item on stack
	grammar_stack.push_back({ gr::prog, node_current });

	// Parsing
	while (grammar_stack.size() && tkn_it != tkn_end) {
		print_stack();

		if (holds_alternative<tkn>(grammar_stack.back().first) &&
			get<tkn>(grammar_stack.back().first) == tkn_it->type) {
			node_current->children.push_back(*tkn_it);
			//grammar_stack.pop_back();
		} else if (holds_alternative<gr>(grammar_stack.back().first)) {
			auto g = get<gr>(grammar_stack.back().first);
			if (rules.count({ g, tkn_it->type})) {
				print_rule({ { g, tkn_it->type }, rules[{ g, tkn_it->type }] });
				cout << "\n";

				node_current = grammar_stack.back().second;

				// Go down
				node_current->children.push_back(make_unique<NonTerm>(NonTerm(g)));
				node_current = get<shared_ptr<NonTerm>>(node_current->children.back());
				node_current->children.push_back(*tkn_it);

				grammar_stack.pop_back();

				for (auto r : rules[{ g, tkn_it->type }]) {
					grammar_stack.push_back({ r, node_current });
				}
				grammar_stack.push_back({tkn_it->type, node_current });
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
			cout << "\n";
			root.print();
			break;
		}
	}

	return make_shared<NonTerm>(root);
}

void Parser::print_stack() {
	cout << "On stack: ";
	for (auto s = grammar_stack.rbegin(); s != grammar_stack.rend(); s++) {
		if (holds_alternative<GrammarItem>(s->first))
			cout << get<GrammarItem>(s->first);
		if (holds_alternative<Tokenizer::TokenType>(s->first))
			cout << get<Tokenizer::TokenType>(s->first);
		cout << " ";
	}
	cout << "\n";
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
