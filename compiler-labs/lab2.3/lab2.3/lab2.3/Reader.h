#pragma once
#include <string>

using namespace std;

/*******************************************************************************
This class provides an interface to read the string symbol-by-symbol
*******************************************************************************/
class Reader {
public:
	Reader() {};
	Reader(string& str);
	const string value();
	char get();
	char next();
private:
	size_t pos = 0;
	string* str = nullptr;
};

