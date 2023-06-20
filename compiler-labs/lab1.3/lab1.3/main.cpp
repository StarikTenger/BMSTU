#include "Scanner.h"
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

	Scanner scanner(str);
	
	while (auto tkn = scanner.next_token()) {
		tkn->print();
		cout << "\n";
	}
}