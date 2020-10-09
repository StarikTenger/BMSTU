#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Queue {
    int *buff;
    int buff_size;
    int size;
    int first;
    int last;
};

struct Queue InitQueue(int size) {
    struct Queue queue;
    queue.buff_size = size;
    queue.size = 0;
    queue.buff = malloc(sizeof(int) * size);
    queue.first = queue.last = 0;
    return queue;
}

void DeleteQueue(struct Queue *queue) {
    free(queue->buff);
}

int QueueEmpty(struct Queue *queue) {
    return !queue->size;
}

int Dequeue(struct Queue *queue) {
    queue->last ++;
    queue->last %= queue->buff_size;
    int val = queue->buff[queue->last];
    queue->size--;
    return val;
}

void Enqueue(struct Queue *queue, int val) {
    queue->size ++;
    if (queue->size == queue->buff_size) {
        int *buff_new = malloc(sizeof(int) * queue->buff_size * 2);
        for (int i = 0; !QueueEmpty(queue); i++)
            buff_new[i] = Dequeue(queue);
        free(queue->buff);
        queue->buff = buff_new;
        queue->last = -1;
        queue->size = queue->buff_size;
        queue->first = queue->size - 1;
        queue->buff[queue->first] = val;
        queue->buff_size *= 2;
        return;
    }
    queue->first++;
    queue->first %= queue->buff_size;
    queue->buff[queue->first] = val;
}


int main(int argc, char **argv) {
    struct Queue queue = InitQueue(4);

    int n;
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        char command[10];
        scanf("%s", command);

        if (!strcmp(command, "EMPTY")) {
            printf(QueueEmpty(&queue) ? "true\n" : "false\n");
        }
        if (!strcmp(command, "ENQ")) {
            int x;
            scanf("%d", &x);
            Enqueue(&queue, x);
        }
        if (!strcmp(command, "DEQ")) {
            printf("%d\n", Dequeue(&queue));
        }

    }

    DeleteQueue(&queue);

    return 0;
}