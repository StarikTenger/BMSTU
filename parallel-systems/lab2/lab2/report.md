***hardcode.h***
```c++
#pragma once

int block_size = 10;
```
main.cpp
```c++
#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <stdexcept>
#include "random.h"
#include "Matrix.h"
#include "Timer.h"

using namespace std;

struct Equation {
	// Ax = b
	Matrix coefficients; // A
	Vector right_part; // b
};

class Solver {
public:
	void solve(const Equation& _eq) {
		eq = _eq;
		init();
		for (int i = 0; i < step_limit && !should_stop(); i++) {
			//print();
			step();
		}
	}

	void print() {
		for (int i = 0; i < x.size(); i++) {
			cout << x(i) << " ";
		}
	}

private:
	Equation eq;
	Vector r;
	Vector z;
	Vector x;
	int step_limit = 1000;
	double alpha = 0;
	double beta = 0;
	double eps = 1e-5;
	void init() {
		x = Vector(eq.right_part.size());
		for (int i = 0; i < x.size(); i++) {
			x(i) = random_float(-2, 2, 2);
		}
		//eq.right_part.print();
		r = eq.right_part - eq.coefficients * x;
		z = r;
		//cout << "r\n";
		//r.print();
	}

	void step() {
		//cout << "\nStep start " << (r * r) << " " << ((eq.coefficients * z) * z) << "\n";
		alpha = (r * r) / ((eq.coefficients * z) * z);
		x = x + alpha * z;
		auto r1 = r - alpha * (eq.coefficients * z);
		beta = (r1 * r1) / (r * r);
		z = r1 + beta * z;
		r = r1;
	}

	bool should_stop() {
		auto err = deviation(r) / deviation(eq.right_part);
		//cout << err << "\n";
		return err < eps;
	}

	double deviation(const Vector& vec) {
		double sum = 0;
		for (int i = 0; i < vec.size(); i++) {
			sum += vec(i) * vec(i);
		}
		return sqrt(sum);
	}
};

int main(int* argc, char** argv) {
	int n = 2000;
	Equation eq;
	eq.coefficients = Matrix(n, n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i == j) {
				eq.coefficients(i, j) = 2.0;
			}
			else {
				eq.coefficients(i, j) = 1.0;
			}
		}
	}

	eq.right_part = Vector(n);
	eq.right_part.fill(n + 1);

	Timer timer;

	int takes = 10;
	int sum_time = 0;
	for (int i = 0; i < 10; i++) {
		Solver solver;
		timer.start("record " + to_string(i));
		solver.solve(eq);
		sum_time += timer.finish();
	}
	cout << "Averrage: " << (float)sum_time / takes;
}

// -- Results --
// 8 threads 1162.3
// 4 threads 1426.5
// 2 threads 2392
// 1 thread  4588.5 
```
Matrix.cpp
```c++
#include "Matrix.h"
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <omp.h>

Matrix::Matrix() {}

Matrix::Matrix(int rows, int columns) : __rows(rows), __columns(columns) {
	__data = new double[rows * columns];
	for (int i = 0; i < columns * rows; i++) {
		__data[i] = 0;
	}
}

Matrix::~Matrix() {
	if (__data) {
		delete __data;
	}
}

Matrix::Matrix(const Matrix& m) {
	__rows = m.__rows;
	__columns = m.__columns;
	__data = new double[m.__rows * m.__columns];
	std::copy(m.__data, m.__data + m.__rows * m.__columns, __data);
}

Matrix& Matrix::operator=(const Matrix& m) {
	if (this == &m) {
		return *this;
	}

	__rows = m.__rows;
	__columns = m.__columns;
	delete __data;
	__data = new double[m.__rows * m.__columns];
	std::copy(m.__data, m.__data + m.__rows * m.__columns, __data);
	return *this;
}

void Matrix::fill(const double& value) {
	for (int i = 0; i < __columns * __rows; i++) {
		__data[i] = value;
	}
}

double Matrix::operator()(const int& i, const int& j) const {
	return __data[i * __columns + j];
}

double& Matrix::operator()(const int& i, const int& j) {
	return __data[i * __columns + j];
}

int Matrix::rows() const {
	return __rows;
}

int Matrix::columns() const {
	return __columns;
}

Vector::Vector() {}

Vector::Vector(int size) : __size(size) {
	__data = Matrix(__size, 1);
}

void Vector::fill(const double& value) {
	__data.fill(value);
}

double Vector::operator()(const int& i) const {
	return __data(i, 0);
}

double& Vector::operator()(const int& i) {
	return __data(i, 0);
}

int Vector::size() const {
	return __size;
}

void Vector::print() const {
	for (int i = 0; i < size(); i++) {
		std::cout << (*this)(i) << " ";
	}
	std::cout << "\n";
}

double operator*(const Vector& l, const Vector& r) {
	if (l.size() != r.size()) {
		throw std::invalid_argument("size mismatch");
	}

	double res = 0;
	for (int i = 0; i < l.size(); i++) {
		res += l(i) * r(i);
	}

	return res;
}

Vector operator*(double l, const Vector& r) {
	Vector v = r;
	for (int i = 0; i < v.size(); i++) {
		v(i) *= l;
	}
	return v;
}

Vector operator+(const Vector& l, const Vector& r) {
	if (l.size() != r.size()) {
		throw std::invalid_argument("size mismatch");
	}

	Vector v = l;
	for (int i = 0; i < v.size(); i++) {
		v(i) += r(i);
	}
	return v;
}

Vector operator-(const Vector& l, const Vector& r) {
	if (l.size() != r.size()) {
		throw std::invalid_argument("size mismatch");
	}

	Vector v = l;
	for (int i = 0; i < v.size(); i++) {
		v(i) -= r(i);
	}
	return v;
}

Vector operator*(const Matrix& l, const Vector& r) {
	if (l.columns() != r.size()) {
		throw std::invalid_argument("size mismatch");
	}

	Vector v(l.rows());

#pragma omp parallel for num_threads(1)
	for (int i = 0; i < l.rows(); i++) {
		double sum = 0;
		for (int j = 0; j < l.columns(); j++) {
			sum += l(i, j) * r(j);
		}
		v(i) = sum;
	}

	return v;
}

```
Matrix.h
```c++
#pragma once

class Matrix {
public:
	// Default constructor
	Matrix();

	// Constructor
	Matrix(int rows, int columns);

	// Destructor
	~Matrix();

	// Copy constructor
	Matrix(const Matrix& m);

	Matrix& operator=(const Matrix& m);

	// Fill matrix with value
	void fill(const double& value);

	// Assign matrix element
	double operator()(const int& i, const int& j) const;
	double& operator()(const int& i, const int& j);

	int rows() const;
	int columns() const;

	friend class Vector;
private:
	int __rows = 0;
	int __columns = 0;
	double* __data = nullptr;
};

class Vector {
public:
	// Default constructor
	Vector();

	// Constructor
	Vector(int size);

	// Fill vector with value
	void fill(const double& value);

	// Assign vector element
	double operator()(const int& i) const;
	double& operator()(const int& i);

	int size() const;

	// Dot product
	friend double operator*(const Vector& l, const Vector& r);

	// Multiply vector by number
	friend Vector operator*(double l, const Vector& r);

	// Summ
	friend Vector operator+(const Vector& l, const Vector& r);

	// Difference
	friend Vector operator-(const Vector& l, const Vector& r);

	// Multiply matrix by vector
	friend Vector operator*(const Matrix& l, const Vector& r);

	void print() const;
private:
	int __size = 0;
	Matrix __data;
};
```
random.h
```c++
#pragma once
#include <cmath>
#include <cstdlib>
#include <sys/timeb.h>

long long binpow(long long a, int b) {
	if (b == 0)
		return 1;
	long long ans = binpow(a, b / 2);
	return ans * ans * ((b % 2) ? a : 1);
}

void random_seed(int seed) {
	srand(seed);
}
int random_int(int min, int max) {
	return rand() % (max + 1 - min) + min;
}
float random_float(float min, float max, int digits) {
	float k = binpow(10, digits);
	return float(random_int(int(min * k), int(max * k))) / k;
}
```
Timer.h
```c++
#pragma once
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <deque>
#include <string>
#include <sys/timeb.h>

int get_milli_count() {
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

class Timer {
private:
	std::deque<std::pair<int, std::string>> records;

public:
	Timer() {}

	void start(std::string record_name) {
		records.push_back({ get_milli_count(), record_name });
	}

	int finish() {
		int time_now = get_milli_count();
		if (records.empty()) {
			std::cerr << "Unable to finish: timer is not set";
			return 0;
		}
		int delta_time = time_now - records.back().first;
		std::cout << records.back().second << " : " << delta_time << "ms\n";

		auto time_error = get_milli_count() - time_now;
		for (auto& record : records) {
			record.first -= time_error;
		}

		return delta_time;
	}
};
```
