#include "StackRational.h"
#include <cstring>

void StackRational::expand() {
	Cell* data_new = new Cell[max_len * 2];
	std::memmove(data_new, data, max_len * sizeof(Cell));
	max_len *= 2;
	delete data;
	data = data_new;
}

StackRational::StackRational(int _max_len) {
	max_len = _max_len;
	data = new Cell[max_len];
}

StackRational::~StackRational() {
	delete data;
}

Rational StackRational::top() {
	return data[len - 1].val;
}

Rational StackRational::min() {
	return data[len - 1].min_val;
}

int StackRational::size() {
	return len;
}

void StackRational::push(Rational x) {
	if (len + 1 >= max_len)
		expand();
	Cell cell;
	cell.val = x;
	cell.min_val = x;
	if (len > 0 && data[len].min_val < x)
		cell.min_val = data[len].min_val;
	data[len] = cell;
	len++;
}

void StackRational::pop() {
	len--;
}

Rational StackRational::operator[](int i) {
	return data[len - i - 1].val;
}

