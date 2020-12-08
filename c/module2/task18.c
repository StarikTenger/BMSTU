#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#define MAX_VAL 100000
#define EPS 1e-4

struct Rational {
    double val;
};

struct Rational Rational_normalize(struct Rational a) {
    return a;
}

struct Rational Rational_init(int a, int b) {
    struct Rational r;
    r.val = log2((double)a / (double)b);
    return Rational_normalize(r);
}

struct Rational Rational_mult(struct Rational a, struct Rational b) {
    struct Rational r;
    r.val = a.val + b.val;
    return Rational_normalize(r);
}

struct Rational Rational_divide(struct Rational a, struct Rational b) {
    struct Rational r;
    r.val = a.val - b.val;
    return Rational_normalize(r);
}

int  Rational_compare(struct Rational a, struct Rational b) {
    double a_val = a.val;
    double b_val = b.val;
    if (a_val + EPS < b_val)
        return -1;
    if (a_val > b_val + EPS)
        return 1;
    return 0;
}

int main() {  
    int n;
    scanf("%d", &n);
    struct Rational *arr = malloc(n * sizeof(struct Rational));
    for (int i = 0; i < n; i++) {
        int a, b;
        scanf("%d/%d", &a, &b);
        arr[i] = Rational_init(a, b);
    }

    int i = 0, j = 0;
    int i_max = 0, j_max = 0;
    struct Rational product_current = Rational_init(1, 1);
    struct Rational product_max = arr[0];

    while (i < n) {
        product_current = Rational_mult(product_current, arr[i]);

        struct Rational product_current_prev = product_current;
        if (Rational_compare(product_current, product_max) == 1){
            product_max = product_current;
            i_max = i;
            j_max = j;
        }
        i++;
        if (Rational_compare(Rational_init(1, 1), product_current_prev) == 1) {
            product_current = Rational_init(1, 1);
            j = i;
        }

    }

    printf("%d %d", j_max, i_max);

    free(arr);
}