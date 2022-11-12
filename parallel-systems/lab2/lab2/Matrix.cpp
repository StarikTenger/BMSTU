#include "Matrix.h"
#include <stdexcept>
#include <cstring>
#include <iostream>

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

	for (int i = 0; i < l.rows(); i++) {
		double sum = 0;
		for (int j = 0; j < l.columns(); j++) {
			sum += l(i, j) * r(j);
		}
		v(i) = sum;
	}

	return v;
}
