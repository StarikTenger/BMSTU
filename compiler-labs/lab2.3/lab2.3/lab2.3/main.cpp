#include "Tokenizer.h"
#include "Parser.h"
#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main() {
	ifstream t("input.txt");
	std::stringstream buffer;
	buffer << t.rdbuf();
	string str = buffer.str();
	
	Tokenizer tokenizer(str);
	auto tokens = tokenizer.tokenize();
	for (const auto& tkn : tokens) {
		cout << tkn << "\n";
	}

	Parser(tokens).parse();
}