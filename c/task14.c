#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int split(char *s, char ***words);

#define INITIAL_SIZE 128

char *getstring()
{
    char *s;
    int flag, len = 0, size = INITIAL_SIZE;

    s = (char*)malloc(INITIAL_SIZE);
    if (s == NULL) return NULL;

    for (;;) {
        if (fgets(s+len, size-len, stdin) == NULL) {
            free(s);
            return NULL;
        }

        len += (int)strlen(s+len);
        if (s[len-1] == '\n') break;

        char *new_s = (char*)malloc(size *= 2);
        if (new_s == NULL) {
            free(s);
            return NULL;
        }

        memcpy(new_s, s, len);
        free(s);
        s = new_s;
    }

    s[len-1] = 0;
    return s;
}

void printword(char *s)
{
    printf("\"");
    for (;;) {
        char c = *s++;
        switch (c) {
        case 0:
            printf("\"\n");
            return;
        case '\a':
            printf("\\a");
            break;
        case '\b':
            printf("\\b");
            break;
        case '\f':
            printf("\\f");
            break;
        case '\n':
            printf("\\n");
            break;
        case '\r':
            printf("\\r");
            break;
        case '\t':
            printf("\\t");
            break;
        case '\v':
            printf("\\v");
            break;
        case '\\':
            printf("\\\\");
            break;
        case '\"':
            printf("\\\"");
            break;
        default:
            printf(c >= 0x20 && c <= 0x7E ? "%c" : "\\x%02x", c);
        }
    }
}

int main()
{
    char *s = getstring();
    if (s == NULL) return 1;

    char **words;
    int n = split(s, &words);
    free(s);
    for (int i = 0; i < n; i++) printword(words[i]);
    for (int i = 0; i < n; i++) free(words[i]);
    free(words);
    return 0;
}

int split(char *s, char ***words) {
    int words_number = 0;
    int len = 0;
    for (int i = 0, j = 0, w = 0;; i++, j++) {
        if (i == 0)
            while (s[i] == ' ') i++;
        len = i;
        if (s[i] == ' ' || s[i] == '\0') {           
            if (j != 0) {
                words_number++;
            }
            j = -1;
            if (s[i] == '\0')
            break;
        }
    }
    char *buff = malloc(len * sizeof(char));
    char **words_array = malloc(words_number * sizeof(char*));
    for (int i = 0, j = 0, w = 0; i <= len; i++, j++) {
        if (i == 0)
            while (s[i] == ' ') i++;
        if (s[i] == ' ' || s[i] == '\0') {
            if (j != 0) {
                buff[j] = '\0';
                j++;
                char *word = malloc(sizeof(char) * j);
                memmove(word, buff, sizeof(char) * j);
                words_array[w] = word;
                w++;
            }
            j = -1;
        } else {
            buff[j] = s[i];
        }
    }
    free(buff);
    *words = words_array;
    return words_number;
}