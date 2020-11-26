#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Date { 
    int Day, Month, Year; 
}; 

int get_key(struct Date date, int key) {
    if (key == 0)
        return date.Day - 1;
    if (key == 1)
        return date.Month - 1;
    return date.Year - 1970;
}

void distribution_sort(int key, int m, struct Date *arr, int n) {
    int *count = malloc(m * sizeof(int));
    for (int i = 0; i < m; i++)
        count[i] = 0;
    for (int i = 0; i < n; i++) 
        count[get_key(arr[i], key)]++;
    for (int i = 1; i < m; i++) 
        count[i] += count[i - 1];
    
    struct Date *arr_copy = malloc(n * sizeof(struct Date));
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
    struct Date *arr = malloc(n * sizeof(struct Date));
    for (int i = 0; i < n; i++)
        scanf("%d%d%d", &arr[i].Year, &arr[i].Month, &arr[i].Day);

    distribution_sort(0, 31, arr, n);
    distribution_sort(1, 12, arr, n);
    distribution_sort(2, 2030 - 1970 + 1, arr, n);

    for (int i = 0; i < n; i++)
        printf("%d %d %d\n", arr[i].Year, arr[i].Month, arr[i].Day);

    free(arr);
}