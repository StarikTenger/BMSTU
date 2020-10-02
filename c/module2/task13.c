#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *prefix_function (char *s) {
	int n = strlen(s);//0; for (;s[n] != '\0'; n++);
	int *pi = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) pi[i] = 0;
	for (int i = 1; i < n; i++) {
		int j = pi[i -1];
		while (j > 0 && s[i] != s[j])
			j = pi[j - 1];
		if (s[i] == s[j])  ++j;
		pi[i] = j;
	}
	return pi;
}

int main(int argc, char **argv) {
    char *s = argv[1];
    char *t = argv[2];

    int len_s = strlen(s);
    int len_t = strlen(t);

    char* str = malloc((len_s + len_t + 1 + 1) * sizeof(char));
    int i = 0;
    for(;i < len_s; i++) str[i] = s[i];
    str[len_s] = '$';
    for(;i < len_s + len_t; i++) str[i + 1] = t[i - len_s];
    str[len_s + len_t + 1] = '\0'; 

    int len = strlen(str);
    int *pref = prefix_function(str);

    int is_pref = 1;
    for (int i = len - 1; i > len_s;) {
        if (pref[i] == 0){
            is_pref = 0;
            break;
        }
        i -= pref[i];
    }

    printf(is_pref ? "yes" : "no");

    free(str);
    free(pref);

    return 0;
}