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
 
void bubblesort(unsigned long, 
    int (*)(unsigned long, unsigned long), 
    void (*)(unsigned long, unsigned long)); 
 
int main(int argc, char **argv) 
{ 
    int i, n; 
    scanf("%d", &n); 

    array = (int*)malloc(n * sizeof(int)); 
    for (i = 0; i < n; i++) scanf("%d", array+i); 

    bubblesort(n, compare, swap); 
    for (i = 0; i < n; i++) printf("%d␣", array[i]); 
    printf("\n"); 

    free(array); 
    return 0; 
} 

void bubblesort(unsigned long nel, 
        int (*compare)(unsigned long i, unsigned long j), 
        void (*swap)(unsigned long i, unsigned long j)) 
    {
    int sorted_left = 0;
    int sorted_right = nel;
    int count = 0;
    for (int j = 0; j < nel; j++) {
        count = 0;
        int right = sorted_right;
        for (int i = sorted_left + 1; i < sorted_right; i++) {
            if (compare(i - 1, i) == 1) {
                swap(i - 1, i);
                count++;
                right = i;
            }
        }
        sorted_right = right;
        if (!count)
            break;

        count = 0;
        int left = sorted_left;
        for (int i = sorted_right - 1; i > sorted_left; i--) {
            if (compare(i - 1, i) == 1) {
                swap(i - 1, i);
                left = i;
                count++;
            }
        }
        sorted_left = left;
        if (!count)
            break;
    }
} 