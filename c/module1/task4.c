#include <stdio.h>
#include <stdlib.h>

#define LEN 'Z' - 'A' + 'z' - 'a' + 1

int get_bit(long num, int bit) {
    num >>= bit;
    return num % 2; 
}

void set_bit(long *num, int bit) {
    long mask = 1;
    mask <<= bit;
    *num = (*num) | mask;
}

int get_index(char c) {
    if (c <= 'Z')
        return c - 'A';
    return c - 'a' + 'Z' - 'A' + 1; 
}

char get_symbol(int i) {
    if (i <= 'Z' - 'A')
        return (char)i + 'A';
    return (char)i + 'a' + 'A' - 'Z' - 1;
}

long input_string() {
    long letters = 0;

    for(;;) {
        char c;
        scanf("%c", &c);
        if (c == '\0' || c == ' ' || c == '\n' || c < 'A' || c > 'z' || (c > 'Z' && c < 'a'))
            return letters;
        set_bit(&letters, get_index(c));
    }

    return letters;
}

int main() {
    long letters_a = input_string();
    long letters_b = input_string();

    for (int i = 0; i <= LEN; i++) {
        if (get_bit(letters_a, i) && get_bit(letters_b, i))
            printf("%c", get_symbol(i));
    }

	return 0;
}