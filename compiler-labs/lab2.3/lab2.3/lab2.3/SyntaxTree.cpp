#include <map>
#include <iostream>
#include <string>
#include "SyntaxTree.h"

ostream& operator<<(ostream& os, const GrammarItem& gri) {
	map<GrammarItem, string> grnames = {
		{GrammarItem::prog, "prog"},
		{GrammarItem::left, "left"},
		{GrammarItem::expr, "expr"},
		{GrammarItem::symbol, "symbol"},
		{GrammarItem::end, "end"},
	};
	os << grnames[gri];
	return os;
}

void NonTerm::print(int depth) const {
	auto tabulation = [&](int n = 0) {for (int i = 0; i < depth + n; i++) cout << "|  "; };
	tabulation();
	cout << type << "\n";
	for (const auto& c : children) {
		if (holds_alternative<Tokenizer::Token>(c)) {
			tabulation(1);
			cout << get<Tokenizer::Token>(c) << "\n";
		}
		else {
			get<shared_ptr<NonTerm>>(c)->print(depth + 1);
		}
	}
}
