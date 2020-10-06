#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int gcd (int a, int b) {
	while (b) {
		a %= b;
		int c = a;
        a = b;
        b = c;
	}
	return a;
}

int *prefix_function (char *s) {
	int n = strlen(s);
	int *pi = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) 
        pi[i] = 0;
	for (int i = 1; i < n; i++) {
		int j = pi[i -1];
		while (j > 0 && s[i] != s[j])
			j = pi[j - 1];
		if (s[i] == s[j])  j++;
		pi[i] = j;
	}
	return pi;
}

int main(int argc, char **argv) {
    char *str = argv[1];
    int len_str = strlen(str);

    int *pref = prefix_function(str);

    for (int i = 0; i < len_str; i++) {
        int g = gcd((i + 1), pref[i]);
        int num = (i + 1) / g;
        if (pref[i] && i + 1 == pref[i] + g)
            printf("%d %d\n", i + 1, num);
    }

    free(pref);
    return 0;
}