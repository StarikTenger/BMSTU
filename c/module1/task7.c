#include <stdio.h>
#include <stdlib.h>

int main() {
    short k;
    int n;
    scanf("%hd %d", &k, &n);
    n++;
    
    char* values = malloc(sizeof(char) * n);
    // Filling values
    for (int i = 0; i < n; i++) {
        values[i] = 1;
    }

    // Algorithm
    for (int i = 2; i < n; i++) {
        if (values[i] != 1)
            continue;
        int k = 2;
        for (int j = i * 2; j < n; j += i, k++)
            values[j] = values[i] + values[k];
    }

    // Output
    for (int i = 2; i < n; i++) {
        //printf("%d) %hd\n", i, values[i]);
        if (values[i] == k)
            printf("%d ", i);
    }

    free(values);
}