#include <iostream>
#include <string>
#include <math.h>
#include <algorithm>
#include "random.h"
#include "Timer.h"

using  fp_type = float;

template <int N, int M>
class Matrix {
private:
	fp_type* data;

public:
	Matrix() {
		data = new fp_type[N * M];
		for (int i = 0; i < N * M; i++) {
			data[i] = 0;
		}
	}

	Matrix (const Matrix<N, M>& m) {
		data = new fp_type[N * M];
		memcpy(data, m.data, N * M * sizeof(fp_type));
	}

	~Matrix() {
		delete data;
	}

	void randomize(float val_min, float val_max) {
		for (int i = 0; i < N * M; i++) {
			data[i] = random_float(val_min, val_max, 2);
		}
	}

	void display() {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				auto str = std::to_string((*this)(i, j));
				std::cout << str << std::string(std::max(2, (int)(10 - str.length())), ' ');
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}

	fp_type& operator()(int i, int j) {
		return data[i * M + j];
	}
};

template <int N, int M, int P>
fp_type multiply_vectors(Matrix<N, M>& m1, Matrix<M, P>& m2, int row, int column) {
	fp_type sum = 0;
	for (int i = 0; i < M; i++) {
		sum += m1(row, i) * m2(i, column);
	}
	return sum;
}

template <int N, int M, int P>
Matrix<N, P> multiply(Matrix<N, M> m1, Matrix<M, P> m2) {
	Matrix<N, P> res;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < P; j++) {
			res(i, j) = multiply_vectors(m1, m2, i, j);
		}
	}
	return res;
}

int main() {
	random_seed(10000);

	Matrix<1000, 1000> m1;
	Matrix<1000, 1000> m2;
	m1.randomize(-10, 10);
	m2.randomize(-10, 10);

	Timer timer;
	timer.start("Casual multiplication 1000x1000");
	auto m3 = multiply(m1, m2);
	timer.finish();

	//m1.display();
	//m2.display();
	//auto m3 = multiply(m1, m2);
	//m3.display();
	return 0;
}