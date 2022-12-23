#include <iostream>
#include <string>
#include <math.h>
#include <algorithm>
#include <vector>
#include <thread>
#include "random.h"
#include "Timer.h"
#include "Table.h"

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
				std::cout << str << std::string(std::max(2, (int)(15 - str.length())), ' ');
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

template <int N, int M, int P>
void multiply_rows(Matrix<N, M>& m1, Matrix<M, P>& m2, Matrix<M, P>& mr, int l, int r) {
	for (int i = l; i < r; i++) {
		for (int j = 0; j < P; j++) {
			mr(i, j) = multiply_vectors(m1, m2, i, j);
		}
	}
}

template <int N, int M, int P>
Matrix<N, P> multiply_treads(Matrix<N, M> m1, Matrix<M, P> m2, int threads_number) {
	Matrix<N, P> res;
	int region_size = N / threads_number;

	std::vector<std::thread> threads;

	for (int l = 0; l < N; l += region_size) {
		int r = std::min(l + region_size, N);
		threads.push_back(std::thread([&](int l, int r) { 
			multiply_rows(m1, m2, res, l, r); 
		}, l, r));
	}

	for (auto& t : threads) {
		t.join();
	}

	return res;
}

template <int N>
void calculate(Table& table) {
	std::cout << "Start testing " << N << "\n";

	Matrix<N, N> m1;
	Matrix<N, N> m2;
	m1.randomize(-10, 10);
	m2.randomize(-10, 10);

	Timer timer;

	std::vector<int> _v; _v.push_back(N);
	table.add_row(_v);
	for (int i = 1; i < N; i *= 2) {
		timer.start(std::to_string(i) + " threads");
		multiply_treads(m1, m2, i);
		auto time = timer.finish();
		table.add_row({ i, time });
	}
}

int main() {
	int seed = 36046;
	random_seed(36046);

	Table table;
	calculate<128>(table);
	calculate<256>(table);
	calculate<512>(table);
	//calculate<1024>(table);
	//calculate<2048>(table);

	table.save("result.xls");
	return 0;
}