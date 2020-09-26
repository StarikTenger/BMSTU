#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main() {
    int m, n;
    scanf("%d%d", &m, &n);

    int* mins = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i ++)
        mins[i] = INT_MAX;

    int saddle_i = -1,
        saddle_j = -1;

    for (int i = 0; i < m; i++) {
        int max = INT_MIN;
        for (int j = 0; j < n; j++) {
            int val;
            scanf("%d", &val);

            if (val > max && val < mins[j]) {
                saddle_i = i;
                saddle_j = j;
                max = val;
                mins[j] = val;
                continue;
            }

            if (val >= max) {
                max = val;
                if (i == saddle_i)
                    saddle_i = -1;
            }
            if (val <= mins[j]) {
                mins[j] = val;
                if (j == saddle_j)
                    saddle_j = -1;
            }
        }
    }

    if (saddle_i == -1 || saddle_j == -1)
        printf("none");
    else
        printf("%d %d", saddle_i, saddle_j);

    free(mins);
}