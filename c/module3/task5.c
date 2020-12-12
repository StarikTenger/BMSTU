#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOWEST_PRIORITY -1000000

struct Elem {
    int value;
    int index;
    int priority;
};

struct Elem Elem_init(int value, int priority, int index) {
    struct Elem elem;
    elem.value = value;
    elem.priority = priority;
    elem.index = index;
    return elem;
}

void swap(struct Elem *a, struct Elem *b) {
    struct Elem c = *a;
    *a = *b;
    *b = c;
}

// Heap functions

int sift_down(struct Elem *arr, int size, int i) {
    int size_max = 0;
    while (i < size / 2) {
        int left = i * 2 + 1;
        int right = left + 1;
        int j = left;
        if (right < size && arr[right].priority > arr[left].priority) 
            j = right;
        if (arr[i].priority >= arr[j].priority)
            return size_max;
        swap(&arr[i], &arr[j]);
        i = j;
        size_max = i;
    }
    return size_max;
}

void sift_up(struct Elem *arr, int size, int i) {
    while (i > 0 && arr[i].priority > arr[(i - 1) / 2].priority) {
        swap(&arr[i], &arr[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// Priority queue

struct Priority_queue {
    struct Elem *arr;
    int size_max;
    int size;
};

struct Priority_queue Priority_queue_init(int size_max) {
    struct Priority_queue queue;
    queue.size_max = size_max;
    queue.size = 0;
    queue.arr = malloc(sizeof(struct Elem) * size_max);
    for (int i = 0; i < size_max; i++) 
        queue.arr[i].priority = LOWEST_PRIORITY;
    return queue;
}

void Priority_queue_push(struct Priority_queue *queue, struct Elem value) {
    queue->arr[queue->size++] = value;
    sift_up(queue->arr, queue->size_max, queue->size - 1);
}

struct Elem Priority_queue_max(struct Priority_queue *queue) {
    return queue->arr[0];
}

void Priority_queue_pop(struct Priority_queue *queue) {
    queue->arr[0].priority = LOWEST_PRIORITY;
    queue->size = sift_down(queue->arr, queue->size_max, 0);
    //queue->size--;
}

int main() {
    int n;
    scanf("%d", &n);
    int **arrays = malloc(sizeof(int*) * n);
    int *sizes = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &sizes[i]);
        arrays[i] = malloc(sizeof(int) * sizes[i]);
    }
    
    for (int i = 0; i < n; i++)
        for (int j = 0; j < sizes[i]; j++)
            scanf("%d", &arrays[i][j]);

    struct Priority_queue queue = Priority_queue_init(n + 1);
    for (int i = 0; i < n; i++)
        if (sizes[i])
            Priority_queue_push(&queue, Elem_init(arrays[i][0], -arrays[i][0], i));
    
    int *indicies = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++)
        indicies[i] = 0;
    
    while (queue.size) {
        struct Elem elem = Priority_queue_max(&queue);
        /*for (int i = 0; i < queue.size_max; i++) 
            printf("%d ", queue.arr[i].priority);
        printf("\n");*/
        printf("%d ", elem.value);
        indicies[elem.index]++;
        Priority_queue_pop(&queue);
        if (indicies[elem.index] < sizes[elem.index])
            Priority_queue_push(&queue, Elem_init(
                arrays[elem.index][indicies[elem.index]],
                -arrays[elem.index][indicies[elem.index]],
                elem.index));
    }

    for (int i = 0; i < n; i++)
        free(arrays[i]);
    free(arrays);
    free(sizes);
    free(indicies);
    free(queue.arr);
}