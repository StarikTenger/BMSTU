#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

char get_bit(int num, int bit) {
    num >>= bit;
    return num % 2; 
}

void set_bit(int *num, int bit) {
    int mask = 1;
    mask <<= bit;
    *num = (*num) | mask;
}

int *tree_build(int *arr, int n) {
    int *tree = malloc(sizeof(int) * n * 2);
    for (int i = 0; i < n; i++)
        tree[n + i] = arr[i];
    
    for (int i = n - 1; i > 0; i--)
        tree[i] = tree[i * 2] ^ tree[i * 2 + 1];

    return tree;
}

long tree_sum(int *tree, int n, int left, int right) {
    left += n;
    right += n;

    long sum = 0;

    if (left == right)
        sum = tree[left];

    while (left < right) {
        if (left % 2 && left + 1 < right)
            sum ^= tree[left++];            
        if (!(right % 2)  && left < right - 1)
            sum ^= tree[right--];     

        if (right - left <= 1) {
            sum ^= tree[right] ^ tree[left];
            break;
        }

        left /= 2;
        right /= 2;
    }  

    int count_ones = 0;
    for (int i = 0; i < 'z' - 'a'; i++)
        count_ones += get_bit(sum, i);

    return (count_ones <= 1);
}

void tree_update(int *tree, int n, int k, int val) {
    k += n;
    tree[k] = val;
    while (k / 2 > 0) {
        k /= 2;
        tree[k] = tree[k * 2] ^ tree[k * 2 + 1];
    }
}

void update(int *tree, int n, int k, char *substr) {
    int len = strlen(substr);
    for (int i = 0; i < len; i++) {
        int val = 0;
        set_bit(&val, substr[i] - 'a');
        tree_update(tree, n, k + i, val);
    }
}

int main(int argc, char **argv) {  
    char str[1000000];
    scanf("%s", str);

    int n = strlen(str);
    int n_new = 1;
    while (n_new < n)
        n_new *= 2;

    int *arr = malloc(sizeof(int) * n_new);
    for (int i = 0; i < n; i++) {
        arr[i] = 0;
        set_bit(&arr[i], str[i] - 'a');
    }
    for (int i = n; i < n_new; i++)
        arr[i] = 0;

    int *tree = tree_build(arr, n_new);

    int m;
    scanf("%d", &m);
    for(int i = 0; i < m; i++) {
        char buff[3];
        scanf("%s", buff);
        
        if (buff[0] == 'H') {
            int l, r;
            scanf("%d%d", &l, &r);
            printf(tree_sum(tree, n_new, l, r) ? "YES\n" : "NO\n");
        }
        else {
            int k;
            char s[1000000];
            scanf("%d%s", &k, s);
            update(tree, n_new, k, s);
        }
    }

    free(arr);
    free(tree);

    return 0;
}