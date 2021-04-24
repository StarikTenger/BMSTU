#pragma once
#include <iostream>

struct Rational {
	int nom = 0;
	int denom = 1;

	Rational(int _nom = 0, int _denom = 0);
	
	Rational normalize();

	Rational operator+(const Rational& x);
	Rational operator-(const Rational& x);
	Rational operator*(const Rational& x);
	Rational operator/(const Rational& x);
};

bool operator<(Rational a, Rational b);
Rational min(const Rational& a, const Rational& b);

