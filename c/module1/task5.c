#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, k;
    scanf("%d%d", &n, &k);

    // Buffer
    long* buff = malloc(sizeof(long) * k);
    int start_pos = 0; // Buffer's first

    // Initial sum 
    long sum = 0;
    for (int i = 0; i < k; i++) {
        long elem;
        scanf("%ld", &elem);
        sum += elem;

        // Filling buffer
        buff[i] = elem;
    }

    // Calculating maximum sum
    long sum_max = sum;
    int sum_max_i = 0;
    for (int i = k; i < n; i++) {
        long elem;
        scanf("%ld", &elem);
        sum += elem;
        sum -= buff[start_pos];

        // Cicle shift of buffer
        buff[start_pos] = elem;
        start_pos++;
        start_pos = start_pos % k;

        // Comparing sum_max to sum
        if (sum > sum_max) {
            sum_max = sum;
            sum_max_i = i - k + 1;
        }
    }

    printf("%d", sum_max_i);

    free(buff);
    return 0;
}