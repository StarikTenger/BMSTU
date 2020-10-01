#include <stdio.h>
#include <stdlib.h>

int main() {
    int n1, n2;
    // Input first array
    scanf("%d", &n1);
    int* arr1 = malloc(sizeof(int) * n1);
    for (int i = 0; i < n1; i ++)
        scanf("%d", &arr1[i]);
    // Input second array
    scanf("%d", &n2);
    int* arr2 = malloc(sizeof(int) * n2);
    for (int i = 0; i < n2; i ++)
        scanf("%d", &arr2[i]);

    int i1 = 0,
        i2 = 0;

    while (i2 < n2 || i1 < n1) {

        if (i1 < n1 && (i2 >= n2 || arr1[i1] <= arr2[i2])) {
            printf("%d ", arr1[i1]);
            i1++;
        }
        if (i2 < n2 && (i1 >= n1 || arr2[i2] <= arr1[i1])) {
            printf("%d ", arr2[i2]);
            i2++;
        }
    }

    free(arr1);
    free(arr2);
}