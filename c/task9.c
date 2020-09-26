#include <stdio.h>
#include <stdlib.h>

int array[] = {
        1000000000,
        2000000000,
        3000000000,
        4000000000,
        5000000000
};

void revarray(void *base, unsigned long nel, unsigned long width) {
    for (unsigned long i = 0; i  < nel; i++) {
        
    }
}

int main(int argc, char **argv) {
        revarray(array, 5, sizeof(int));

        int i;
        for (i = 0; i < 5; i++) {
                printf("%d\n", array[i]);
        }

        return 0;
}