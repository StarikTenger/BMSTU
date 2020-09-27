#include  <stdio.h>

int strdiff(char *a, char *b);

int main(int argc, char **argv)
{
        char s1[1000], s2[1000];
        scanf("%s", s1);
        scanf("%s", s2);
        printf("%d\n", strdiff(s1, s2));

        return 0;
}

int strdiff(char *a, char *b) {
    int bits = 0;
    for (int i = 0;; i++) {
        bits += 8;
        if (a[i] != b[i]) {
            char c = (a[i])^(b[i]);
            while (c != 0)
                c <<= 1, bits--;
            break;
        }
        if (a[i] == '\0' || b[i] == '\0') 
            return -1;
    }
    return bits;
}