#pragma once
#include "Rational.h"

class StackRational {
private:
	struct Cell {
		Rational val;
		Rational min_val;
	};

	Cell* data;
	int max_len = 1;
	int len = 0;

	void expand();
public:
	StackRational(int _max_len = 1);
	~StackRational();
	
	Rational top();
	Rational min();
	int size();
	void push(Rational);
	void pop();

	Rational operator[](int i);
};

