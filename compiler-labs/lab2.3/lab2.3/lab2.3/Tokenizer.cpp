#include "Tokenizer.h"
#include <iostream>
#include <unordered_map>

Tokenizer::Tokenizer(const string& str) {
	str_it = str_begin = str.begin();
	str_end = str.end();
}

optional<Tokenizer::Token> Tokenizer::read_token() {
	skip();
	if (end_encountered) {
		return nullopt;
	}
	if (str_it == str_end) {
		end_encountered = true;
		return Token(TokenType::Eof, "", pos());;
	}
	switch (*str_it) {
	case '(':
		str_it++;
		return Token(TokenType::ParL, "(", pos() - 1);
	case ')':
		str_it++;
		return Token(TokenType::ParR, ")", pos() - 1);
	case '|':
		str_it++;
		return Token(TokenType::Bar, "|", pos() - 1);
	case '.':
		str_it++;
		return Token(TokenType::Dot, ".", pos() - 1);
	case '=':
		str_it++;
		return Token(TokenType::Eq, "=", pos() - 1);
	default: {
		size_t start_pos = pos();
		if (*str_it) {
			string acc = "";
			while (str_it != str_end && !space_symbol(*str_it) &&
				string("().|").find(*str_it) == string::npos) {
				if (*str_it == '\\')
					str_it++;
				acc += *str_it;
				str_it++;
			}
			return Token(acc == "axiom" ? TokenType::Axiom : TokenType::String,
				acc, start_pos);
		}
		else {
			return Token(TokenType::String, "" + *(str_it++), start_pos);
		}
	}
	}
	return nullopt;
}

vector<Tokenizer::Token> Tokenizer::tokenize() {
	vector<Token> tokens;
	optional<Token> token;
	while (token = read_token()) {
		tokens.push_back(*token);
	}
	return tokens;
}

size_t Tokenizer::pos() {
	return str_it - str_begin;
}

bool Tokenizer::space_symbol(char c) {
	return c == ' ' || c == '\n' || c == '\t';
}

void Tokenizer::skip() {
	bool comment = false;
	while (str_it != str_end && (space_symbol(*str_it) || *str_it == ';')) {
		if (*str_it == ';') {
			while ((str_it != str_end && *str_it != '\n')) str_it++;
		}
		if (str_it != str_end)
			str_it++;
	}
}

ostream& operator<<(ostream& os, const Tokenizer::TokenType& tkn) {
	unordered_map<Tokenizer::TokenType, string> names = {
		{Tokenizer::TokenType::ParL, "ParL"},
		{Tokenizer::TokenType::ParR, "ParR"},
		{Tokenizer::TokenType::Bar, "Bar"},
		{Tokenizer::TokenType::Dot, "Dot"},
		{Tokenizer::TokenType::Eq, "Eq"},
		{Tokenizer::TokenType::String, "String"},
		{Tokenizer::TokenType::Err, "Err"},
		{Tokenizer::TokenType::Eof, "Eof"},
	};
	os << names[tkn];
	return os;
}

ostream& operator<<(ostream& os, const Tokenizer::Token& tkn) {
	os << tkn.type;
	os << "[";
	os << to_string(tkn.pos);
	os << "]: ";
	os << tkn.value;
	return os;
}
