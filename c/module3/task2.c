#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_max(int a, int b) {
    return a > b ? a : b;
}
int get_min(int a, int b) {
    return a < b ? a : b;
}

struct Elem {
    int value;
    struct Elem *next;
};

struct Mystack {
    struct Elem *top;
};

int Mystack_top(struct Mystack *s) {
    int value  = s->top->value;
    struct Elem *top = s->top;
    s->top = s->top->next;
    free(top);
    return value;
}

void Mystack_push(struct Mystack *s, int value) {
    struct Elem *top = malloc(sizeof(struct Elem));
    top->value = value;
    top->next = s->top;
    s->top = top;
}

void Mystack_add(struct Mystack *s) {
    Mystack_push(s, Mystack_top(s) + Mystack_top(s));
}

void Mystack_sub(struct Mystack *s) {
    Mystack_push(s, Mystack_top(s) - Mystack_top(s));
}

void Mystack_mul(struct Mystack *s) {
    Mystack_push(s, Mystack_top(s) * Mystack_top(s));
}

void Mystack_div(struct Mystack *s) {
    Mystack_push(s, Mystack_top(s) / Mystack_top(s));
}

void Mystack_max(struct Mystack *s) {
    Mystack_push(s, get_max(Mystack_top(s), Mystack_top(s)));
}

void Mystack_min(struct Mystack *s) {
    Mystack_push(s, get_min(Mystack_top(s), Mystack_top(s)));
}

void Mystack_neg(struct Mystack *s) {
    Mystack_push(s, -Mystack_top(s));
}

void Mystack_dup(struct Mystack *s) {
    int n = Mystack_top(s);
    Mystack_push(s, n);
    Mystack_push(s, n);
}

void Mystack_swap(struct Mystack *s) {
    int a = Mystack_top(s);
    int b = Mystack_top(s);
    Mystack_push(s, a);
    Mystack_push(s, b);
}

int main(int argc, char **argv) {
    int n;
    scanf("%d", &n);

    struct Mystack stack;
    stack.top = 0;

    for (int i = 0; i < n; i++) {
        char command[10];
        scanf("%s", command);

        if (!strcmp(command, "CONST")) {
            int x;
            scanf("%d", &x);
            Mystack_push(&stack, x);
        }
        if (!strcmp(command, "ADD"))
            Mystack_add(&stack);
        if (!strcmp(command, "SUB"))
            Mystack_sub(&stack);
        if (!strcmp(command, "MUL"))
            Mystack_mul(&stack);
        if (!strcmp(command, "DIV"))
            Mystack_div(&stack);
        if (!strcmp(command, "MAX"))
            Mystack_max(&stack);
        if (!strcmp(command, "MIN"))
            Mystack_min(&stack);
        if (!strcmp(command, "NEG"))
            Mystack_neg(&stack);
        if (!strcmp(command, "DUP"))
            Mystack_dup(&stack);
        if (!strcmp(command, "SWAP"))
            Mystack_swap(&stack);
    }

    printf("%d", Mystack_top(&stack));
}