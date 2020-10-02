#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


long *tree_build(long *arr, int n) {
    long *tree = malloc(sizeof(long) * n * 2);
    for (int i = 0; i < n; i++)
        tree[n + i] = arr[i];
    
    for (int i = n - 1; i > 0; i--)
        tree[i] = tree[i * 2] + tree[i * 2 + 1];

    return tree;
}

long tree_sum(long *tree, int n, int left, int right) {
    left += n;
    right += n;

    long sum = 0;

    if (left == right)
        sum = tree[left];

    while (left < right) {
        if (left % 2 && left + 1 < right)
            sum += tree[left++];            
        if (!(right % 2)  && left < right - 1)
            sum += tree[right--];     

        if (right - left <= 1) {
            sum += tree[right] + tree[left];
            break;
        }

        left /= 2;
        right /= 2;
    }
    
    return sum;
}


void tree_update(long *tree, int n, int k, int val) {
    k += n;
    tree[k] = val;
    while (k / 2 > 0) {
        k /= 2;
        tree[k] = tree[k * 2] + tree[k * 2 + 1];
    }
}

void update(long* arr, long *tree, int n, int n_new, int k, int val) {
    arr[k] = val;
    for (int i = k - 1; i <= k + 1; i++) {
        if (i < 0 || i >= n)
            continue;
        long v = (long)((i == 0 || arr[i] >= arr[i - 1]) && (i == n - 1 || arr[i] >= arr[i + 1]));
        tree_update(tree, n_new, i, v);
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
        arr[i] = 0;

    long *peaks = malloc(sizeof(long) * n_new);
    for (int i = 0; i < n; i++)
        peaks[i] =  (long)((i == 0 || arr[i] >= arr[i - 1]) && (i == n - 1 || arr[i] >= arr[i + 1]));

    long *tree = tree_build(peaks, n_new);

    // for (int i = 0; i < n_new * 2; i++)
    //     printf("%lu ", tree[i]);
    // printf("\n");

    int m;
    scanf("%d", &m);
    for(int i = 0; i < m; i++) {
        char buff[4];
        scanf("%s", buff);
        int l, r;
        scanf("%d%d", &l, &r);

        if (buff[0] == 'P')
            printf("%ld\n", tree_sum(tree, n_new, l, r));
        else
            update(arr, tree, n, n_new, l, r);
    }

    free(arr);
    free(peaks);
    free(tree);

    return 0;
}