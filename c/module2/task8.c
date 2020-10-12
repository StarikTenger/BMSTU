#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(int *a, int *b) {
    int c = *a;
    *a = *b;
    *b = c;
}

void selection_sort(int *arr, int left, int right) {
    for (int i = left; i < right; i ++)
        for (int j = i + 1; j < right; j++)
            if (arr[i] > arr[j])
                swap(&arr[i], &arr[j]);
}

int iteration(int *arr, int left, int right, int mid) {
    int *arr_new = malloc(sizeof(int) * (right - left));
    int k = 0;
    for (int i = left; i < right; i++)
        if (arr[i] < arr[mid])
            arr_new[k] = arr[i], k++;
    int mid_i = k;
    for (int i = left; i < right; i++)
        if (arr[i] >= arr[mid])
            arr_new[k] = arr[i], k++;
    for (int i = left; i < right; i++)
        arr[i] = arr_new[i -left];
    free(arr_new);
    return mid_i;
}

void quick_sort(int *arr, int left, int right, int m) {
    for(;;) {
        int mid = left + iteration(arr, left, right, left) + 1;

        if (right - left < 2)
            return;

        if (right - left < m) {
            selection_sort(arr, left, right);
            return;
        }
        
        if (mid - left < right - mid) {
            quick_sort(arr, left, mid, m);
            left = mid;
        } else {
            quick_sort(arr, mid, right, m);
            right = mid;
        }
    }
}

int main(int argc, char **argv) {
    int n, m;
    scanf("%d %d", &n, &m);
    int *arr = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);
    
    quick_sort(arr, 0, n, m);

    for (int i = 0; i < n; i++)
        printf ("%d ", arr[i]);

    free(arr);

    return 0;
}