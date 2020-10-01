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

int overlap(const char *a, const char *b) {
    int len_a = strlen(a),
        len_b = strlen(b);
    char* str = malloc((len_a + len_b + 1 + 1) * sizeof(char));
    int i = 0;
    for(;i < len_a; i++) str[i] = a[i];
    str[len_a] = '$';
    for(;i < len_a + len_b; i++) str[i + 1] = b[i - len_a];
    str[len_a + len_b + 1] = '\0';

    //printf("%s\n", str);
    int *pref = prefix_function(str);
    int val = pref[len_a + len_b ];
    free(str);
    free(pref);
    return val;
}

char get_bit(short num, short bit) {
    num >>= bit;
    return num % 2; 
}

void set_bit(short *num, short bit) {
    short mask = 1;
    mask <<= bit;
    *num = (*num) | mask;
}

char* matrix_iterate(char* matrix, int n, int i, int j) {
    return &(matrix[i * n + j]);
}

char *make_overlap_matrix(char **strings, int n) {
    char *arr = malloc(sizeof(char) * n * n);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            *matrix_iterate(arr, n, i, j) = overlap(strings[i], strings[j]);

    return arr;
}

void brutforce (char *overlap_matrix, char **strings, int n, int *val_min, short checked, char *order, char k, int value) {
    if (k > 0) {
        value += strlen(strings[order[k - 1]]);
        if (k > 1)
            value -= *matrix_iterate(overlap_matrix, n, order[k - 1], order[k - 2]);
    }

    if (value >= *val_min)
        return;

    if (k == n) {
        if (value < *val_min)
            *val_min = value;
        return;
    }

    for (int i = 0; i < n; i++) {
        if (get_bit(checked, i))
            continue;
        //char *order_new = malloc(sizeof(char) * n);     
        //for (int j = 0; j < k; j++) order_new[j] = order[j];
        short checked_new = checked;        
        set_bit(&checked_new, i);
        order[k] = i;
        
        brutforce(overlap_matrix, strings, n, val_min, checked_new, order, k + 1, value);
        //free(order_new);
    }
    
    return;
}

int shortest_superstring(char **strings, int n) {

    short checked = 0;
    char *order = malloc(sizeof(char) * n);

    char *overlap_matrix;
    int val_min = 10000;

    overlap_matrix = make_overlap_matrix(strings, (char)n);

    brutforce(overlap_matrix, strings, n, &val_min, checked, order, 0, 0);

    free(order);
    free(overlap_matrix);
    return val_min;
}



int main() {
    int n;
    scanf("%d", &n);
    char **strings = malloc(sizeof(char*) * n);
    for (int i = 0; i < n; i++) {
        char *str = malloc(sizeof(char) * 100);
        scanf("%s", str);
        strings[i] = str;
        //printf("%ld\n", strlen(str));
    }
    printf("%d", shortest_superstring(strings, n));

    for (int i = 0; i < n; i++) 
        free(strings[i]);
    free(strings);

    return 0;
}