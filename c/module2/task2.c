#include <stdio.h>
#include <stdlib.h>

void count (int *counter, int *arr, int n, int k, int sum) {
    if (sum > 0 && !(sum & (sum - 1)))
        (*counter) ++;
    if (k >= n)
        return;
    for (int i = k; i < n; i++) {
        count(counter, arr, n, i + 1, sum + arr[i]);
    }
}

int main() {
    int n;
    scanf("%d", &n);
    int *arr = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    int counter = 0;
    count(&counter, arr, n, 0, 0);

    printf("%d", counter);

    free(arr);
	return 0;
}