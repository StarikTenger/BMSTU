#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Elem { 
    struct Elem *next; 
    int value;
    int value_max;
};

struct Stack {
    struct Elem *front; 
};

struct Stack Stack_init() {
    struct Stack stack;
    stack.front = 0;
    return stack;
}

int Stack_front(struct Stack *stack) {
    return stack->front->value;
}

int Stack_max(struct Stack *stack) {
    return stack->front->value_max;
}

int Stack_is_empty(struct Stack *stack) {
    return !stack->front;
}

void Stack_pop_front(struct Stack *stack) {
    struct Elem *old_front = stack->front;
    stack->front = stack->front->next;
    free(old_front);
}

void Stack_push_front(struct Stack *stack, int value) {
    struct Elem *new_front = malloc(sizeof(struct Elem));
    new_front->value = value;
    new_front->value_max = (Stack_is_empty(stack) || value > stack->front->value_max) ? value : stack->front->value_max;
    new_front->next = stack->front;
    stack->front = new_front;
}

void Stack_clear(struct Stack *stack) {
    while (stack->front)
        Stack_pop_front(stack);
}

struct Queue {
    struct Stack stack_in;
    struct Stack stack_out;
};

struct Queue Queue_init() {
    struct Queue queue;
    queue.stack_in = Stack_init();
    queue.stack_out = Stack_init();
    return queue;
}

int Queue_back(struct Queue *queue) {
    if (Stack_is_empty(&queue->stack_out)) {
        while (!Stack_is_empty(&queue->stack_in)) {
            Stack_push_front(&queue->stack_out, Stack_front(&queue->stack_in));
            Stack_pop_front(&queue->stack_in);
        }
    }
    return Stack_front(&queue->stack_out);
}

int Queue_max(struct Queue *queue) {
    if (Stack_is_empty(&queue->stack_in))
        return Stack_max(&queue->stack_out);
    if (Stack_is_empty(&queue->stack_out))
        return Stack_max(&queue->stack_in);
    int max_in = Stack_max(&queue->stack_in);
    int max_out = Stack_max(&queue->stack_out);
    return max_in > max_out ? max_in : max_out;
}

int Queue_is_empty(struct Queue *queue) {
    return Stack_is_empty(&queue->stack_in) && Stack_is_empty(&queue->stack_out);
}

void Queue_pop_back(struct Queue *queue) {
    Queue_back(queue);
    Stack_pop_front(&queue->stack_out);
}

void Queue_push_front(struct Queue *queue, int value) {
    Stack_push_front(&queue->stack_in, value);
}

void Queue_clear(struct Queue *queue) {
    while (!Queue_is_empty(queue))
        Queue_pop_back(queue);
}

int main() {
    struct Queue queue = Queue_init();

    int n;
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        char command[10];
        scanf("%s", command);

        if (!strcmp(command, "EMPTY")) {
            printf(Queue_is_empty(&queue) ? "true\n" : "false\n");
        }
        if (!strcmp(command, "ENQ")) {
            int x;
            scanf("%d", &x);
            Queue_push_front(&queue, x);
        }
        if (!strcmp(command, "DEQ")) {
            printf("%d\n", Queue_back(&queue));
            Queue_pop_back(&queue);
        }
        if (!strcmp(command, "MAX")) {
            printf("%d\n", Queue_max(&queue));
        }
    }

    Queue_clear(&queue);

    return 0;
}