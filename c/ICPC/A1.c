#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    for(;;) {
        int a, b, c;
        scanf("%d%d%d", &a, &b, &c);
        if (a == 0 && b == 0 && c == 0)
            break;
        if (a + c == 2 * b)
            printf("AP %d\n", c + b - a);
        else
            printf("GP %d\n", c * b / a);
    }
    return 0;
}