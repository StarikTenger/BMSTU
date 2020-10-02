#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

long gcd(long a, long b) {
    while (b) {
		a %= b;
        long c = a;
		a = b;
        b = c;
	}
	return a;
}

long *tree_build(long *arr, int n) {
    long *tree = malloc(sizeof(long) * n * 2);
    for (int i = 0; i < n; i++)
        tree[n + i] = arr[i];
    
    for (int i = n - 1; i > 0; i--)
        tree[i] = gcd(tree[i * 2], tree[i * 2 + 1]);

    return tree;
}

long tree_gcd(long *tree, int n, int left, int right) {
    left += n;
    right += n;

    long gcd_elem = tree[left];

    while (left < right) {
        gcd_elem = gcd(gcd_elem, gcd(tree[left], tree[right]));

        if (left % 2 && left + 1 < right)
            left++;            
        if (!(right % 2)  && left < right - 1)
            right--;

        if (right - left <= 1)
            break;

        left /= 2;
        right /= 2;
    }
    gcd_elem = gcd(gcd_elem, gcd(tree[left], tree[right]));
    
    return gcd_elem;
}

void tree_update(long *tree, int n, int k, int val) {
    k += n;
    tree[k] = val;
    while (k / 2 > 0) {
        k /= 2;
        tree[k] = gcd(tree[k * 2], tree[k * 2 + 1]);
    }
}

int main(int argc, char **argv) {  
    int n;
    scanf("%d", &n);
    int n_new = 1;
    while (n_new < n)
        n_new *= 2;

    long *arr = malloc(sizeof(long) * n_new);
    for (int i = 0; i < n; i++) {
        scanf("%ld", &arr[i]);
        arr[i] = (arr[i] > 0) ? arr[i] : -arr[i];
    }
    for (int i = n; i < n_new; i++)
        arr[i] = 1;

    long *tree = tree_build(arr, n_new);

    int m;
    scanf("%d", &m);
    for(int i = 0; i < m; i++) {        
        int l, r;
        scanf("%d%d", &l, &r);

        printf("%ld\n", tree_gcd(tree, n_new, l, r));
    }

    free(arr);
    free(tree);

    return 0;
}