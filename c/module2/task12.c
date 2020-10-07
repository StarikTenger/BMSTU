#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char iter(char *s, char *t, int len_s, int i) {
    if (i < len_s)
        return s[i];
    return t[i - len_s - 1];
}

// Symbol on len_s index is not equal to any other
int comp(char *s, char *t, int len_s, int i, int j) { 
    return i != len_s && j != len_s && iter(s, t, len_s, i) == iter(s, t, len_s, j);
}

int *prefix_function (char *s, char *t, int len_s, int n) {
	int *pi = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) pi[i] = 0;
	for (int i = 1; i < n; i++) {
		int j = pi[i - 1];
		while (j > 0 && !comp(s, t, len_s, i, j))
			j = pi[j - 1];
		if (comp(s, t, len_s, i, j))  j++;
		pi[i] = j;
	}
	return pi;
}

int main(int argc, char **argv) {
    char *s = argv[1];
    char *t = argv[2];

    int len_s = strlen(s);
    int len_t = strlen(t);

    int len = len_s + len_t + 1;
    int *pref = prefix_function(s, t, len_s, len);

    for (int i = len_s + 1; i < len; i++)
        if (pref[i] == len_s)
            printf("%d ", (i - len_s * 2 ));
    free(pref);

    return 0;
}