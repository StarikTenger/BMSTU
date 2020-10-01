#include <stdio.h>
#include <stdlib.h>

long fact(long n, long k) {
	long val = 1;
	for (long i = n; i > (n - k); i--) {
		if (i <= 0)
			continue;
		val *= i;
	}
	return val;
}

long power(long n, long k) {
	long val = 1;
	for(long i = 0; i < k; i++) {
		val *= n;
	}
	return val;
}

int main() {
	long n, k, x0;
	scanf("%ld%ld%ld", &n, &k, &x0);

	long result = 0;
	//long a_prev = 0; // Ы
	for (long i = 0; i < n - k + 1; i++) {
		long a = 0;
		scanf("%ld", &a);

		long p = n - i; // Current power
		long step = a * fact(p, k);

		//printf("%ld) %ld = %ld + %ld\n", i, (result * x0 + step), result, step);

		result = result * x0 + step;
	}

	printf("%ld\n", result);

	return 0;
}