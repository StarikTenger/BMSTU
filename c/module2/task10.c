#include <stdio.h>
#include <stdlib.h>
#include <string.h>

union Int32 { 
    int x; 
    unsigned char bytes[4]; 
}; 

int get_key(union Int32 num, int key) {
    if (key == 3)
        if (num.x < 0)
            return (int)num.bytes[key] - 128;
        else
            return 128 + (int)num.bytes[key];
    return num.bytes[key];
}

void distribution_sort(int key, int m, union Int32 *arr, int n) {
    int *count = malloc(m * sizeof(int));
    for (int i = 0; i < m; i++)
        count[i] = 0;
    for (int i = 0; i < n; i++) 
        count[get_key(arr[i], key)]++;
    for (int i = 1; i < m; i++) 
        count[i] += count[i - 1];
    
    union Int32 *arr_copy = malloc(n * sizeof(union Int32));
    for (int i = 0; i < n; i++)
        arr_copy[i] = arr[i];
    
    for (int i = n - 1; i >= 0; i--) {
        int k = get_key(arr_copy[i], key);
        arr[--count[k]] = arr_copy[i];
    }
    
    free(count);
    free(arr_copy);
}

int main() {
    int n;
    scanf("%d", &n);
    union Int32 *arr = malloc(n * sizeof(union Int32));
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i].x);
        memmove(arr[i].bytes, &arr[i].x, sizeof(int));
    }

    for (int i = 0; i < 4; i++)
        distribution_sort(i, 256, arr, n);

     for (int i = 0; i < n; i++)
        printf("%d ", arr[i].x);
    
    free(arr);
}