#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void csort(char *src, char *dest) { 
    int len = strlen(src);
    int *word_lens = malloc(len * sizeof(int));
    int word_number = 0;
    for (int i = len - 1; i >= 0; i--) {
        if (src[i] == ' ')
            word_lens[i] = 0;
        else
            word_lens[i] = (i == len - 1) ? 1 : (word_lens[i + 1] + 1);

        if (i == 0 && src[i] != ' ' || i != 0 && src[i - 1] == ' ' && src[i] != ' ')
            word_number++;
    }

    int *word_begins = malloc(word_number * sizeof(int));
    for (int i = 0, w = 0; i < len; i++)
        if (i == 0 && src[i] != ' ' || i != 0 && src[i - 1] == ' ' && src[i] != ' ')
            word_begins[w++] = i;

    int *count = malloc(word_number * sizeof(int));
    for (int i = 0; i < word_number; i++)
        count[i] = 0;

    for (int i = 0; i < word_number - 1; i++) {
        for (int j = i + 1; j < word_number; j++) {
            if (word_lens[word_begins[j]] < word_lens[word_begins[i]])
                count[i]++;
            else
                count[j]++;
        }
    }

    int *order = malloc(word_number * sizeof(int));
    for (int i = 0; i < word_number; i++) {
        order[count[i]] = word_begins[i];
    }

    int letter = 0;
    for (int i = 0; i < word_number; i++) {
        //printf("%d ", order[i]);
        for (int j = order[i]; j < order[i] + word_lens[order[i]]; j++) {
            dest[letter++] = src[j];
        }
        dest[letter++] = ' ';
    }
    dest[letter++] = '\0';

    free(word_lens);
    free(word_begins);
    free(count);
    free(order);
}

int main() {
    char str[1000];
    char out[1000];
    gets(str);
    csort(str, out);
    printf("%s", out);
}