#include <stdio.h>
#include <stdlib.h>

int len = 256;

void input_string(short* letters) {
    for(int i = 0; i < len; i++) 
        letters[i] = 0;

    for(;;) {
        char c;
        scanf("%c", &c);
        if (c == '\0' || c == ' ' || c == '\n' || c < 'A' || c > 'z' || (c > 'Z' && c < 'a'))
            return;
        letters[c] = 1;
    }
}

int main() {
    short* letters_a = malloc(len * sizeof(short));
    short* letters_b = malloc(len * sizeof(short));

    input_string(letters_a);
    input_string(letters_b);

    for (int i = 0; i < len; i++) {
        //printf("%hd %hd\n", letters_a[i], letters_b[i]);
        if (letters_a[i] && letters_b[i])
            printf("%c", (i));
    }

    free(letters_a);
    free(letters_b);

	return 0;
}