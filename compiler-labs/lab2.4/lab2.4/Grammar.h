#pragma once
#include "Tokenizer.h"
#include <map>
#include <optional>
#include <set>
#include <string>
#include <variant>
#include <vector>

using namespace std;

class Grammar {
  public:
	struct Expression;
	using Nonterm = int;
	using Term = string;

	struct Alternative {
		vector<Expression> children;
		Alternative(vector<Expression> children = {}) : children(children) {}
	};
	struct Concatenation {
		vector<Expression> children;
		Concatenation(vector<Expression> children = {}) : children(children) {}
	};
	struct Expression {
		bool kleene_star = 0;
		variant<Alternative, Concatenation, Nonterm, Term> value;
		Expression(){};
		Expression(variant<Alternative, Concatenation, Nonterm, Term> value,
				   bool kleene_star = false)
			: value(value), kleene_star(kleene_star) {}
	};
	using Production = pair<Nonterm, Expression>;

	Grammar(){};
	Grammar(const vector<Tokenizer::Token>& tokens);

  private:
	enum ExprType {
		NONTERM_CODE = 0,
		TERM_CODE = 1,
		CONC_CODE = 2,
		ALT_CODE = 4
	};

	map<Nonterm, Expression> productions;

	Nonterm nonterm_current = 0;
	map<Nonterm, Term> nonterm_to_name;
	map<Term, Nonterm> name_to_nonterm;

	//== Parsers ===============================================================
	using Tkn = Tokenizer::Token;
	using TknType = Tokenizer::TokenType;

	optional<Tkn> scan_token(vector<Tkn>::const_iterator& scanner_position,
							 TknType);

	optional<Nonterm> scan_nonterm(
		vector<Tkn>::const_iterator& scanner_position);

	optional<Term> scan_term(vector<Tkn>::const_iterator& scanner_position);

	optional<Alternative> scan_alternative(
		vector<Tkn>::const_iterator& scanner_position, size_t = 0);

	optional<Concatenation> scan_concatenation(
		vector<Tkn>::const_iterator& scanner_position, size_t = 0);

	optional<Expression> scan_expression(
		vector<Tkn>::const_iterator& scanner_position, size_t = 0);

	optional<Production> scan_production(
		vector<Tkn>::const_iterator& scanner_position);

	void print_production(const Production& prod, int depth = 0);
	void print_expression(const Expression& expr, int depth = 0);
	static void print_tabs(int n);
};
