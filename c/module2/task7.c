#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(int *a, int *b) {
    int c = *a;
    *a = *b;
    *b = c;
}

int less(int a, int b) {
    return abs(a) < abs(b);
}

void insertion_sort(int *arr, int left, int right) {
    for (int i = left + 1; i < right; i++)
        for (int j = i; j > left && less(arr[j], arr[j - 1]); j--)
            swap(&arr[j - 1], &arr[j]);
}

void merge(int *arr, int left, int right, int mid) {
    int *merged = malloc(sizeof(int) * (right - left));
    int i = left,
        j = mid;
    int k = 0;
    while (i < mid || j < right) {
        if (j == right || i != mid && !less(arr[j], arr[i]))
            merged[k] = arr[i], i++, k++;
        else
            merged[k] = arr[j], j++, k++;
    }

    for (int i = left; i < right; i++)
        arr[i] = merged[i - left];

    free(merged);
}

void merge_sort(int *arr, int left, int right) {
    if (left >= right - 1)
        return;

    if (right - left < 5) {
        insertion_sort(arr, left, right);
        return;
    }

    int mid = (left + right) / 2;
    //printf("%d %d\n", left, right);
    merge_sort(arr, left, mid);
    merge_sort(arr, mid, right);
    merge(arr, left, right, mid);
}

int main(int argc, char **argv) {
    int n;
    scanf("%d", &n);
    int *arr = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);
    
    merge_sort(arr, 0, n);

    for (int i = 0; i < n; i++)
        printf ("%d ", arr[i]);

    free(arr);

    return 0;
}