#include "Grammar.h"
#include <iostream>
#include <string>

Grammar::Grammar(const vector<Tokenizer::Token>& tokens) {
	vector<Tkn>::const_iterator scanner_position = tokens.begin();
	while (scanner_position->type != TknType::Eof) {
		auto pr = scan_production(scanner_position);
		if (pr) {
			print_production(*pr);
			cout << "\n";
		} else {
			cerr << "error\n";
			break;
		}
	}
	
	
}

optional<Grammar::Tkn> Grammar::scan_token(
	vector<Tkn>::const_iterator& scanner_position, TknType type) {
	if (scanner_position->type == type) {
		//cout << "scan_token " << *scanner_position << "\n";
		auto return_value = *scanner_position;
		scanner_position++;
		return return_value;
	}
	return nullopt;
}

optional<Grammar::Nonterm> Grammar::scan_nonterm(
	vector<Tkn>::const_iterator& scanner_position) {
	auto it = scanner_position;
	//cout << "scan_nonterm\n";

	if (!scan_token(it, TknType::ParL)) return nullopt;
	//cout << *it << " 1\n";
	string value;
	if (auto token = scan_token(it, TknType::String)) {
		value = token->value;
	} else {
		return nullopt;
	}
	//cout << *it << " 2\n";
	if (!scan_token(it, TknType::ParR)) return nullopt;
	//cout << *it << " 3\n";
	Nonterm id;
	if (name_to_nonterm.count(value)) {
		id = name_to_nonterm[value];
	} else {
		id = nonterm_current++;
		nonterm_to_name[id] = value;
		name_to_nonterm[value] = id;
	}

	scanner_position = it;
	return id;
}

optional<Grammar::Term> Grammar::scan_term(
	vector<Tkn>::const_iterator& scanner_position) {
	auto it = scanner_position;
	//cout << "scan_term\n";

	string value;
	if (auto token = scan_token(it, TknType::String)) {
		value = token->value;
	} else {
		return nullopt;
	}

	scanner_position = it;
	return value;
}

optional<Grammar::Alternative> Grammar::scan_alternative(
	vector<Tkn>::const_iterator& scanner_position, size_t expr_disallowed) {
	auto it = scanner_position;
	if (expr_disallowed & ALT_CODE) {
		return nullopt;
	}
	//cout << "scan_alternative\n";

	auto expr1 = scan_expression(it, expr_disallowed | ALT_CODE);
	if (!expr1) return nullopt;
	auto bar = scan_token(it, TknType::Bar);
	if (!bar) return nullopt;
	auto expr2 = scan_expression(it);

	if (expr1 && bar && expr2) {
		scanner_position = it;
		return Alternative({*expr1, *expr2});
	}

	return nullopt;
}

optional<Grammar::Concatenation> Grammar::scan_concatenation(
	vector<Tkn>::const_iterator& scanner_position, size_t expr_disallowed) {
	auto it = scanner_position;
	if (expr_disallowed & CONC_CODE) {
		return nullopt;
	}
	//cout << "scan_concatenation\n";

	auto expr1 = scan_expression(it, expr_disallowed | CONC_CODE);
	if (!expr1) return nullopt;
	auto expr2 = scan_expression(it, expr_disallowed | ALT_CODE);

	if (expr1 && expr2) {
		scanner_position = it;
		return Concatenation({*expr1, *expr2});
	}

	return nullopt;
}

optional<Grammar::Expression> Grammar::scan_expression(
	vector<Tkn>::const_iterator& scanner_position, size_t expr_disallowed) {
	auto it = scanner_position;
	//cout << "scan_expression\n";
	//cout << expr_disallowed << "\n";
	if (auto expr = scan_alternative(it, expr_disallowed)) {
		scanner_position = it;
		return Expression(*expr);
	} else if (auto expr = scan_concatenation(it, expr_disallowed)) {
		scanner_position = it;
		return Expression(*expr);
	} else if (auto expr = scan_nonterm(it)) {
		scanner_position = it;
		return Expression(*expr, scan_token(scanner_position, TknType::Star) !=
									 nullopt);
	} else if (auto expr = scan_term(it)) {
		scanner_position = it;
		return Expression(*expr, scan_token(scanner_position, TknType::Star) !=
									 nullopt);
	} else if (scan_token(it, TknType::BracketL)) {
		auto expr_inner = scan_expression(it);
		scan_token(it, TknType::BracketR);
		if (scan_token(it, TknType::Star)) {
			expr_inner->kleene_star = true;
		}
		scanner_position = it;
		return Expression(*expr_inner);
	} 

	return nullopt;
}

optional<Grammar::Production> Grammar::scan_production(
	vector<Tkn>::const_iterator& scanner_position) {
	auto it = scanner_position;

	auto nonterm = scan_nonterm(it);
	scan_token(it, TknType::Eq);
	auto expr = scan_expression(it);

	if (nonterm && expr) {
		scan_token(it, TknType::Dot);
		scanner_position = it;
		return pair<Nonterm, Expression>(*nonterm, *expr);
	}
	return nullopt;
}

void Grammar::print_tabs(int n) {
	for (int i = 0; i < n; i++) {
		cout << "|  ";
	}
}

void Grammar::print_production(const Production& prod, int depth) {
	print_tabs(depth);
	cout << "Production\n";
	print_tabs(depth + 1);
	cout << "Nonterm: ";
	cout << nonterm_to_name[prod.first] << "\n";
	print_expression(prod.second, depth + 1);
}

void Grammar::print_expression(const Expression& expr, int depth) {
	if (expr.kleene_star) {
		print_tabs(depth);
		cout << "*\n";
		depth++;
	}
	if (holds_alternative<Alternative>(expr.value)) {
		print_tabs(depth);
		cout << "Alternative\n";
		for (const auto& c : get<Alternative>(expr.value).children) {
			print_expression(c, depth + 1);
		}
	}
	if (holds_alternative<Concatenation>(expr.value)) {
		print_tabs(depth);
		cout << "Concatenation\n";
		for (const auto& c : get<Concatenation>(expr.value).children) {
			print_expression(c, depth + 1);
		}
	}
	if (holds_alternative<Nonterm>(expr.value)) {
		print_tabs(depth);
		cout << "Nonterm: ";
		cout << nonterm_to_name[get<Nonterm>(expr.value)] << "\n";
	}
	if (holds_alternative<Term>(expr.value)) {
		print_tabs(depth);
		cout << "Term: ";
		cout << get<Term>(expr.value) << "\n";
	}
}
