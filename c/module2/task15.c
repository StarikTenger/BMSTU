#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

char get_bit(short num, short bit) {
    num >>= bit;
    return num % 2; 
}

void set_bit(short *num, short bit) {
    short mask = 1;
    mask <<= bit;
    *num = (*num) | mask;
}

long maxof(long a, long b) {
    return (a > b) ? a : b;
}

long *tree_build(long *arr, int n) {
    long *tree = malloc(sizeof(long) * n * 2);
    for (int i = 0; i < n; i++)
        tree[n + i] = arr[i];
    
    for (int i = n - 1; i > 0; i--)
        tree[i] = maxof(tree[i * 2], tree[i * 2 + 1]);

    return tree;
}

long tree_max(long *tree, int n, int left, int right) {
    left += n;
    right += n;

    long max_elem = LONG_MIN;

    while (left < right) {
        max_elem = maxof(max_elem, maxof(tree[left], tree[right]));

        if (left % 2 && left + 1 < right)
            left++;            
        if (!(right % 2)  && left < right - 1)
            right--;

        if (right - left <= 1)
            break;

        left /= 2;
        right /= 2;
    }
    max_elem = maxof(max_elem, maxof(tree[left], tree[right]));
    
    return max_elem;
}

void tree_update(long *tree, int n, int k, int val) {
    k += n;
    tree[k] = val;
    while (k / 2 > 0) {
        k /= 2;
        tree[k] = maxof(tree[k * 2], tree[k * 2 + 1]);
    }
}

int main(int argc, char **argv) {  
    int n;
    scanf("%d", &n);
    int n_new = 1;
    while (n_new < n)
        n_new *= 2;

    long *arr = malloc(sizeof(long) * n_new);
    for (int i = 0; i < n; i++)
        scanf("%ld", &arr[i]);
    for (int i = n; i < n_new; i++)
        arr[i] = LONG_MIN;

    long *tree = tree_build(arr, n_new);

    int m;
    scanf("%d", &m);
    for(int i = 0; i < m; i++) {
        char buff[3];
        scanf("%s", buff);
        int l, r;
        scanf("%d%d", &l, &r);

        if (buff[0] == 'M')
            printf("%ld\n", tree_max(tree, n_new, l, r));
        else
            tree_update(tree, n_new, l, r);
    }

    free(arr);
    free(tree);

    return 0;
}