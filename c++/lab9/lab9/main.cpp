#include <iostream>
#include<stdarg.h>

template <int D>
class DigitSet {
private:
	int data = 0;
public:
	DigitSet() {}
	DigitSet(int _data) {
		data = _data;
	}

	void add(int n) {
		data |= (1 << n);
	}

	void remove(int n) {
		data &= ~(1 << n);
	}

	friend std::ostream& operator<<(std::ostream& os, const DigitSet<D>& ds) {
		for (int i = 0; i < D; i++) 
			if ((ds.data >> i) & 1)
				os << i << " ";
		os << "\n";
		return os;
	}

	friend DigitSet<D> operator+(const DigitSet<D>& a, int b) {
		DigitSet<D> c = a;
		c.add(b);
		return c;
	}

	friend DigitSet<D> operator-(const DigitSet<D>& a, int b) {
		DigitSet<D> c = a;
		c.remove(b);
		return c;
	}

	friend DigitSet<D>& operator+=(DigitSet<D>& a, int b) {
		a.add(b);
		return a;
	}

	friend DigitSet<D>& operator-=(DigitSet<D>& a, int b) {
		a.remove(b);
		return a;
	}

	friend DigitSet<D> operator+(const DigitSet<D>& a, const DigitSet<D>& b) {
		return DigitSet<D>(a.data | b.data);
	}

	friend DigitSet<D> operator-(const DigitSet<D>& a, const DigitSet<D>& b) {
		return DigitSet<D>(a.data & ~b.data);
	}

	friend DigitSet<D> operator*(const DigitSet<D>& a, const DigitSet<D>& b) {
		return DigitSet<D>(a.data & b.data);
	}

};

int main() {
	DigitSet<10> a;
	a = a + 1 + 2 + 3 + 4;

	DigitSet<10> b;
	b = a - 1 - 2 + 5 + 6 + 7;

	std::cout << "a: " <<  a;
	std::cout << "b: " << b;
	std::cout << "a + b: " << a + b;
	std::cout << "a * b: " << a * b;
	std::cout << "a - b: " << a - b;
	std::cout << "b - a: " << b - a;
}