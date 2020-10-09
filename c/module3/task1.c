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

struct Elem {
    struct Task value;
    struct Elem *next;
};

struct Mystack {
    struct Elem *top;
};

struct Task Mystack_top(struct Mystack *s) {
    struct Task value  = s->top->value;
    struct Elem *top = s->top;
    s->top = s->top->next;
    free(top);
    return value;
}

void Mystack_add(struct Mystack *s, struct Task value) {
    struct Elem *top = malloc(sizeof(struct Elem));
    top->value = value;
    top->next = s->top;
    s->top = top;
}

int iteration(int *arr, int left, int right, int mid) {
    int *arr_new = malloc(sizeof(int) * (right - left));
    int k = 0;
    for (int i = left; i < right; i++)
        if (arr[i] < arr[mid])
            arr_new[k] = arr[i], k++;
    int mid_i = k;
    for (int i = left; i < right; i++)
        if (arr[i] >= arr[mid])
            arr_new[k] = arr[i], k++;
    for (int i = left; i < right; i++)
        arr[i] = arr_new[i -left];
    free(arr_new);
    return mid_i;
}

void quick_sort(int *arr, int n) {
    struct Mystack stack;
    stack.top = 0;

    Mystack_add(&stack, Task_new(0, n));
    while (stack.top) {
        struct Task task = Mystack_top(&stack);

        int mid = task.left + iteration(arr, task.left, task.right, task.left) + 1;

        if (mid > task.left + 1)
            Mystack_add(&stack, Task_new(task.left, mid));
        if (mid + 1 < task.right)
            Mystack_add(&stack, Task_new(mid, task.right));
    }
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