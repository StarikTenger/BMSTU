#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(void *base, size_t width, unsigned long i, unsigned long j) {
    void* buff = malloc(width);
    memmove(buff, ((char*)base + i * width), width);
    memmove(((char*)base + i * width), ((char*)base + j * width), width);
    memmove(((char*)base + j * width), buff, width);
    free(buff);
}

int comp (const void *a1, const void *b1) {
    char *a = *(char**)a1;
    char *b = *(char**)b1;
    //printf("COMPARE %s %s\n", a, b);
    int n_a = 0;
    int n_b = 0;
    int size_a = strlen(a);
    int size_b = strlen(b);
    for (int i = 0; i < size_a; i++)
        if (a[i] == 'a')
            n_a++;
    for (int i = 0; i < size_b; i++)
        if (b[i] == 'a')
            n_b++;

    //printf("COMPARE %s %s\n", a, b);
    if (n_a < n_b)
        return -1;
    if (n_a > n_b)
        return 1;
    return 0;
}

void heapify(void *base, size_t nel, size_t width, int (*compare)(const void *a, const void *b), unsigned long i) {
    for(;;) {
        unsigned long l = 2 * i + 1;
        unsigned long r = l + 1;
        unsigned long j = i;
        if (l < nel && compare(base + i * width, base + l * width) == -1)
            i = l;
        if (r < nel && compare(base + i * width, base + r * width) == -1)
            i = r;
        if (i == j)
            break;
        swap(base, width, i, j);
    }
}

void build_heap(void *base, size_t nel, size_t width, int (*compare)(const void *a, const void *b)) {
    for (long i = nel / 2 - 1; i >= 0; i--)
        heapify(base, nel, width, compare, i);
}

void hsort(void *base, size_t nel, size_t width, int (*compare)(const void *a, const void *b)) { 
    build_heap(base, nel, width, compare);
    for (unsigned long i = nel - 1; i > 0; i--) {
        swap(base, width, 0, i);
        heapify(base, i, width, compare, 0);
    }
    /*for (int i = 0; i < nel; i++) {
        for (int j = i + 1; j < nel; j++) {
            if (compare(base + j * width, base + i * width) == -1)
                swap(base, width, i, j);
        }
    }*/
}

int main() {
    int n;
    scanf("%d", &n);
    char **strs = malloc(n * sizeof(char*));
    for (int i = 0; i < n; i++) {
        char *str = malloc(sizeof(char) * 1000);
        scanf("%s", str);
        strs[i] = str;
    }

    hsort(strs, n, sizeof(char*), comp);

    for (int i = 0; i < n; i++) {
        printf("%s\n", strs[i]);
        free(strs[i]);
    }
    free(strs);
}