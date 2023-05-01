#include "Tokenizer.h"
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
	for (const auto& tkn : tokenizer.tokenize()) {
		cout << tkn << "\n";
	}
}