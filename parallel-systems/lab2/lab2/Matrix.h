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