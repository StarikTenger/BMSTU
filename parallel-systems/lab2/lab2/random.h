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