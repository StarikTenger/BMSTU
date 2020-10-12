#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Task { 
    int left, right; 
}; 

struct Task Task_new(int left, int right) {
    struct Task t;
    t.left = left;
    t.right = right;
    return t;
}


void swap(int *a, int *b) {
    int c = *a;
    *a = *b;
    *b = c;
}

int iteration(int *arr, int left, int right, int mid) {
    int k = left;
    int mid_val = arr[mid];
    for (int i = left; i < right; i++) {
        if (arr[i] < mid_val) {
            while (arr[k] < mid_val)
                k++;
            swap(&arr[i], &arr[k]);
        }
    }
    return k - left;
}

void quick_sort(int *arr, int n) {
    struct Task *stack = malloc(sizeof(struct Task) * n);
    int i = 0;
    stack[i++] = Task_new(0, n);

    while(i) {
        struct Task task = stack[--i];
        for (;;) {
            int mid = task.left + iteration(arr, task.left, task.right, task.left) + 1;
            //printf("%d : %d %d\n", i, task.left, task.right);

            if (mid <= task.left + 1 && mid + 1 >= task.right)
                break;

            if (mid - task.left < task.right - mid) {
                stack[i++] = Task_new(task.left, mid);
                task.left = mid;
            } else {
                stack[i++] = Task_new(mid, task.right);
                task.right = mid;
            }
        }
    }

    free(stack);
}

int main(int argc, char **argv) {
    int n;
    scanf("%d", &n);
    int *arr = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);
    
    quick_sort(arr, n);
    
    for (int i = 0; i < n; i++)
        printf ("%d ", arr[i]);

    free(arr);

    return 0;
}