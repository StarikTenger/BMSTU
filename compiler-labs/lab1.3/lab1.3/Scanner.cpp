#include "Scanner.h"

Scanner::Scanner(const string& str) {
	cur_pos.str_it = str_begin = str.begin();
	str_end = str.end();
}

void Scanner::report_error(string message) {
	cout << "Error (" << cur_pos.line + 1 << ", " << cur_pos.pos << "): " << message << "\n";
}

Scanner::TokenStr* Scanner::scan_str() {
	TokenStr* tkn = new TokenStr();
	tkn->coord.start = cur_pos;
	tkn->attr = "";
	tkn->tag = Str;
	cur_pos++;
	bool error = 0;
	while (!reached_end() && *cur_pos.str_it != '"') {
		if (*cur_pos.str_it == '\\') {
			cur_pos++;
			switch (*cur_pos.str_it) {
			case 'n': tkn->attr += '\n';  break;
			case '"': tkn->attr += '"';  break;
			case 't': tkn->attr += '\t';  break;
			case '\\': tkn->attr += '\\';  break;
			default: report_error("incorrect symbol in string"); error = 1; break;
			}
		}
		else {
			tkn->attr += *cur_pos.str_it;
		}
		cur_pos++;
	}
	tkn->coord.end = cur_pos;
	cur_pos++;
	if (error)
		return nullptr;
	return tkn;
}

Scanner::TokenNum* Scanner::scan_num() {
	TokenNum* tkn = new TokenNum();
	tkn->coord.start = cur_pos;
	tkn->tag = Num;

	string acc = "";
	while (!reached_end() && (is_digit(*cur_pos.str_it) || *cur_pos.str_it == '_')) {
		if (*cur_pos.str_it != '_') {
			acc += *cur_pos.str_it;
		}
		cur_pos++;
	}
	tkn->attr = stoi(acc);
	tkn->coord.end = cur_pos;
	return tkn;
}

Scanner::Token* Scanner::next_token() {
	skip();
	if (reached_end()) {
		return nullptr;
	}
	if (*cur_pos.str_it == '"') {
		if (auto res = scan_str()) {
			return res;
		}
		else {
			cur_pos++;
			return next_token();
		}
	}
	else if (is_digit(*cur_pos.str_it)) {
		if (auto res = scan_num()) {
			return res;
		}
		else {
			cur_pos++;
			return next_token();
		}
	}
	else {
		report_error("unknown symbol");
		cur_pos++;
		return next_token();
	}
	return nullptr;
}

Scanner::Position Scanner::Position::operator++(int) {
	Position backup = *this;
	if (*str_it == '\n') {
		line++;
		pos = 0;
	}
	else {
		pos++;
	}
	str_it++;
	return backup;
}

bool Scanner::is_digit(char c) {
	return c >= '0' && c <= '9';
}

bool Scanner::space_symbol(char c) {
	return c == ' ' || c == '\t' || c == '\n';
}


bool Scanner::reached_end() {
	return cur_pos.str_it == str_end;
}

void Scanner::skip() {
	while (!reached_end() && space_symbol(*cur_pos.str_it)) {
		cur_pos++;
	}
}

void Scanner::Token::print() {
	switch (tag) {
	case Str:
		cout << "Str";
		break;
	case Num:
		cout << "Num";
		break;
	case Eof:
		cout << "Eof";
		break;
	}
	cout << " (" << coord.start.line + 1 << ", " << coord.start.pos << ")-(" << 
		coord.end.line + 1 << ", " << coord.end.pos << "): ";
	print_attr();
	cout << "\n";
}

void Scanner::TokenStr::print_attr() {
	cout << attr;
}

void Scanner::TokenNum::print_attr() {
	cout << attr;
}
