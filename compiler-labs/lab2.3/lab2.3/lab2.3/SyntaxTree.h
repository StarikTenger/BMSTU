#pragma once
#include "Tokenizer.h"
#include <memory>
#include <variant>

using namespace std;

enum class GrammarItem {
	prog, left, expr, symbol, end
};

ostream& operator<<(ostream& os, const GrammarItem& gri);

struct NonTerm {
	GrammarItem type = GrammarItem::end;
	vector<variant<Tokenizer::Token, shared_ptr<NonTerm>>> children = {};
	NonTerm() {}
	NonTerm(GrammarItem type) : type(type){}
	void print(int depth = 0) const;
};
