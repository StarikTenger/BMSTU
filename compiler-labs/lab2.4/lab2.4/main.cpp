#include "Tokenizer.h"
#include "Grammar.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <sstream>
#include <string>

using namespace std;

int main() {
	ifstream t("input.txt");
	std::stringstream buffer;
	buffer << t.rdbuf();
	string str = buffer.str();

	Tokenizer tokenizer(str);
	vector<Tokenizer::Token> tokens = tokenizer.tokenize();
	for (const auto& tkn : tokens) {
		cout << tkn << "\n";
	}

	Grammar gr(tokens);
}