#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOWEST_PRIORITY -1000000000

struct Elem {
    long long value;
    long long priority;
};

struct Elem Elem_init(long long value) {
    struct Elem elem;
    elem.value = value;
    elem.priority = -value;
    return elem;
}

void swap(struct Elem *a, struct Elem *b) {
    struct Elem c = *a;
    *a = *b;
    *b = c;
}

// Heap functions

long long sift_down(struct Elem *arr, long long size, long long i) {
    long long size_max = 0;
    while (i < size / 2) {
        long long left = i * 2 + 1;
        long long right = left + 1;
        long long j = left;
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

void sift_up(struct Elem *arr, long long size, long long i) {
    while (i > 0 && arr[i].priority > arr[(i - 1) / 2].priority) {
        swap(&arr[i], &arr[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// Priority queue

struct Priority_queue {
    struct Elem *arr;
    long long size_max;
    long long size;
    long long number_of_elements;
};

struct Priority_queue Priority_queue_init(long long size_max) {
    struct Priority_queue queue;
    queue.size_max = size_max;
    queue.size = 0;
    queue.number_of_elements = 0;
    queue.arr = malloc(sizeof(struct Elem) * size_max);
    for (long long i = 0; i < size_max; i++) {
        queue.arr[i] = Elem_init(0);
        queue.arr[i].priority = LOWEST_PRIORITY;
    }
    return queue;
}

void Priority_queue_push(struct Priority_queue *queue, struct Elem value) {
    queue->arr[queue->size++] = value;
    sift_up(queue->arr, queue->size_max, queue->size - 1);
    queue->number_of_elements++;
}

struct Elem Priority_queue_max(struct Priority_queue *queue) {
    return queue->arr[0];
}

void Priority_queue_pop(struct Priority_queue *queue) {
    queue->arr[0].priority = LOWEST_PRIORITY;
    queue->size = sift_down(queue->arr, queue->size_max, 0);
    queue->number_of_elements--;
}

long long Priority_queue_size(struct Priority_queue *queue) {
    return queue->number_of_elements;
}

void Priority_queue_free(struct Priority_queue *queue) {
    free(queue->arr);
}

int main() {
    long long n;
    scanf("%lld", &n);
    struct Priority_queue units = Priority_queue_init(n);
    for (long long i = 0; i < n; i++)
        Priority_queue_push(&units, Elem_init(0));

    long long k;
    scanf("%lld", &k);
    long long time_max = 0;
    for (long long i = 0; i < k; i++) {
        long long start, working;
        scanf("%lld %lld", &start, &working);
        long long time = Priority_queue_max(&units).value;
        Priority_queue_pop(&units);
        if (time < start)
            time = start;
        time += working;
        if (time_max < time)
            time_max = time;
        Priority_queue_push(&units, Elem_init(time));
    }

    printf("%lld", time_max);

    // Frees
    Priority_queue_free(&units);
}