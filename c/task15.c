#include <stdio.h>
#include <stdlib.h>

int string_to_int(char *str) {
    int sum = 0;
    int d = 1;
    for (int i = 1; str[i] != '\0'; i++, d *= 10);
    for (int i = 0; str[i] != '\0'; i++, d /= 10) {
        sum += (str[i] - '0') * d;
    }
    return sum;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: frame <height> <width> <text>");
        return 0;
    }

    int height = string_to_int(argv[1]),
        width = string_to_int(argv[2]);
    char *str = argv[3];
    int str_len = 0;
    while (str[str_len] != '\0')
        str_len++;

    if (str_len > width - 2 || height < 3){
        printf("Error");
        return 0;
    }


    // *****
    for (int i = 0; i < width; i++)
        printf("*");
    printf("\n");

    int middle = (height % 2) ? (height / 2) : (height / 2 - 1);
    int padding_L = (width - str_len) / 2 - 1;
    int padding_R = (width - str_len) / 2; if (str_len % 2 == 0) padding_R--;
    if (width % 2)
        if (str_len % 2)
            padding_R--;
        else
            padding_R++;

    // *   *
    for (int i = 1; i < height - 1; i ++) {
        printf("*");
        if (i != middle)
            for (int j = 0; j < width - 2; j++) printf(" ");
        else {
            for (int j = 0; j < padding_L; j++) printf(" ");
            printf("%s", str);
            for (int j = 0; j < padding_R; j++) printf(" ");
        }
        
        printf("*\n");
    }

    // *****
    for (int i = 0; i < width; i++)
        printf("*");
}