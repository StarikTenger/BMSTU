#include "Rational.h"
#include <algorithm>

Rational::Rational(int _nom, int _denom) {
	nom = _nom;
	denom = _denom;
}

Rational Rational::normalize() {
	auto gcd = [](int a, int b) {
		while (b) {
			a %= b;
			std::swap(a, b);
		}
		return a;
	};
	if (nom == 0)
		return {0, 1};
	int sign = nom * denom > 0 ? 1 : -1;
	nom = abs(nom);
	denom = abs(denom);
	int divider = gcd(nom, denom);
	return {sign * nom / divider, denom};
}

Rational Rational::operator+(const Rational& x) {
	return Rational(nom * x.denom + x.nom * denom, denom * x.denom).normalize();
}

Rational Rational::operator-(const Rational& x) {
	return Rational(nom * x.denom - x.nom * denom, denom * x.denom).normalize();
}

Rational Rational::operator*(const Rational& x) {
	return Rational(nom * x.nom, denom * x.denom).normalize();
}

Rational Rational::operator/(const Rational& x) {
	return Rational(nom * x.denom, denom * x.nom).normalize();
}

bool operator<(Rational a, Rational b) {
	return (a - b).normalize().nom < 0;
}

Rational min(const Rational& a, const Rational& b) {
	if (a < b)
		return a;
	return b;
}
