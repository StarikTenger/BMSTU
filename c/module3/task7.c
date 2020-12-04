#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Elem { 
    struct Elem *next; 
    char *word; 
};

struct Elem *list_add(struct Elem *elem, char *word) {
    struct Elem *new_elem = malloc(sizeof(struct Elem));
    new_elem->word = word;

    struct Elem *next = elem->next;
    elem->next = new_elem;
    new_elem->next = next;

    return new_elem;
}

struct Elem *list_swap(struct Elem *elem0) {
    struct Elem *elem1 = elem0->next;
    struct Elem *elem2 = elem1->next;
    struct Elem *elem3 = elem2->next;
    elem0->next = elem2;
    elem1->next = elem3;
    elem2->next = elem1;
    return elem2;
}

int less(struct Elem *a, struct Elem *b) {
    int val_a = strlen(a->word);
    int val_b = strlen(b->word);
    return val_a < val_b;
}

struct Elem *bsort(struct Elem *list) {
    for (;;) {
        int swaps = 0;
        for (struct Elem *elem_j = list; elem_j->next->next; elem_j = elem_j->next) {
            if (less(elem_j->next->next, elem_j->next)) {
                list_swap(elem_j);
                swaps++;
            }
        }
        if(!swaps)
            break;
    }
}

int main() {
    struct Elem *current_elem = malloc(sizeof(struct Elem));
    current_elem->word = 0;
    current_elem->next = 0;
    struct Elem *list_begin = current_elem;

    for(;;) {
        char *word = malloc(sizeof(char) * 1000);
        if (scanf("%s", word) <= 0)
            break;
        current_elem = list_add(current_elem, word);
    }

    bsort(list_begin);


    for (struct Elem *elem = list_begin, *elem_prev = 0; elem->next; elem = elem->next) {
        free(elem_prev);
        printf("%s\n", elem->next->word);
        free(elem->next->word);
        elem_prev = elem;
    }
}