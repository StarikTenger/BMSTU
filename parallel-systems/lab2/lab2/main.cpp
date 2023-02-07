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