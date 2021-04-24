#include <iostream>
#include <vector>

template<int D, int N>
class Number {
private:
	std::vector<unsigned int> digits;
public:
	Number() {
		digits = std::vector<unsigned int>(N);
	}
	Number(std::vector<unsigned int> _digits) {
		digits = std::vector<unsigned int>(N);
		init(_digits);
	}

	unsigned int operator[](int i) {
		return digits[N - i - 1];
	}

	void init(std::vector<unsigned int> _digits) {
		for (int i = _digits.size() - 1; i >= 0; i--)
			digits[i] = _digits[_digits.size() - 1 - i];
	}

	Number operator+(Number b) {
		Number a = *this;
		unsigned int rem = 0;
		for (int i = 0; i < N; i++) {
			a.digits[i] += b.digits[i];
			if (a.digits[i] >= D) {
				a.digits[i + 1] += a.digits[i] - D + 1;
				a.digits[i] %= D;
			}
		}
		return a;
	}

	void display() {
		for (int i = N - 1; i >= 0; i--)
			std::cout << digits[i] << " ";
		std::cout << "\n";
	}
};

int main() {
	Number<3, 10> a({ 2, 2, 2, 2 });
	Number<3, 10> b({ 1 });
	a.display();
	b.display();
	(a + b).display();

}