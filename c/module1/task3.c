#include <stdio.h>
#include <stdlib.h>

long max_of(long a, long b) {
    if (a > b)
        return a;
    return b;
}

long min_of(long a, long b) {
    if (a < b)
        return a;
    return b;
}

int main() {
    int n;
    scanf("%d", &n);

    long left = 0,
        right = 0;

    scanf("%ld%ld", &left, &right);

    for (int i = 1; i < n; i++) {
        long a, b;
        scanf("%ld%ld", &a, &b);
        if (a <= right + 1) {
            if (b > right)
                right = b;
        } else {
            printf("%ld %ld\n", left, right);
            right = b;
            left = a;            
        }
    }
    printf("%ld %ld\n", left, right);

	return 0;
}