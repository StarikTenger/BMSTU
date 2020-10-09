#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int *buff  = malloc(sizeof(int) * 4);
    buff[0] = 1;
    buff[1] = 2;
    buff[2] = 3;
    buff[3] = 4;

    int *change = malloc(sizeof(int));
    memmove(change, buff, sizeof(int));
    memmove(buff, buff + 1, sizeof(int));
    memmove(buff + 1, change, sizeof(int));

    for (int i = 0; i < 4; i++)
        printf("%d ", buff[i]);
}