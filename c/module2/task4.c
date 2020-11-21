#include <stdlib.h> 
#include <stdio.h> 
 
int *array; 
 
int compare(unsigned long i, unsigned long j) 
{ 
    if (i <= j) { 
        printf("COMPARE␣%d␣%d\n", i, j); 
    } else { 
        printf("COMPARE␣%d␣%d\n", j, i); 
    } 

    if (array[i] == array[j]) return 0; 
    return array[i] < array[j] ? -1 : 1; 
} 
 
void swap(unsigned long i, unsigned long j) 
{ 
    if (i <= j) { 
        printf("SWAP␣%d␣%d\n", i, j); 
    } else { 
        printf("SWAP␣%d␣%d\n", j, i); 
    } 

    int t = array[i]; 
    array[i] = array[j]; 
    array[j] = t; 
} 
 
void shellsort(unsigned long, 
    int (*)(unsigned long, unsigned long), 
    void (*)(unsigned long, unsigned long)); 
 
int main(int argc, char **argv) 
{ 
    int i, n; 
    scanf("%d", &n); 

    array = (int*)malloc(n * sizeof(int)); 
    for (i = 0; i < n; i++) scanf("%d", array+i); 

    shellsort(n, compare, swap); 
    for (i = 0; i < n; i++) printf("%d␣", array[i]); 
    printf("\n"); 

    free(array); 
    return 0; 
} 

void shellsort(unsigned long nel, 
    int (*compare)(unsigned long i, unsigned long j), 
    void (*swap)(unsigned long i, unsigned long j)) 
    {
    int a = 1, b = 1;
    int count = 0;
    for (;b < nel; count++) {
        int c = b;
        b = a + b;
        a = c;
    }
    
    int *sequence = malloc(count * sizeof(int));
    a = 1, b = 1;
    for (int i = 0; b < nel; i++) {
        int c = b;
        b = a + b;
        a = c;
        sequence[i] = a;
    }

    for (int k = count - 1; k >= 0; k--) {
        int d = sequence[k];
        //printf("%d\n", d);
        for (int i = 0; i < nel; i++)
            for (int j = i - d; j >= 0 && compare(j, j + d) == 1; j -= d)
                swap(j, j + d);
    }
    
    free(sequence);
} 