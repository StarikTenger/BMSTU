#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LETTERS_NUMBER 26

int get_index(char c) {
    return (int)(c - 'a');
}

struct Node {
    int count;
    int ends;
    int index;
    struct Node *parent;
    int children_size;
    struct Node **children;
};

struct Node Node_init(int children_size) {
    struct Node node;
    node.children_size = children_size;
    node.children = malloc(sizeof(struct Node*) * children_size);
    for (int i = 0; i < children_size; i++)
        node.children[i] = 0;
    node.parent = 0;
    node.count = 0;
    node.ends = 0;
    node.index = 0;
    return node;
}

struct Tree {
    struct Node *root;
};

struct Tree Tree_init() {
    struct Tree tree;
    tree.root = malloc(sizeof(struct Node));
    *tree.root = Node_init(LETTERS_NUMBER);
    return tree;
}

void Tree_destructor_recursive(struct Node *node) {
    if (!node)
        return;
    for (int i = 0; i < node->children_size; i++)
        Tree_destructor_recursive(node->children[i]);
    free(node->children);
    free(node);
}

void Tree_destructor(struct Tree *tree) {
    Tree_destructor_recursive(tree->root);
}

void Tree_insert(struct Tree *tree) {
    //printf("INSERT\n");
    struct Node *node = tree->root;
    char symbol = ' ';
    while (symbol == ' ' || symbol == '\n')
        scanf("%c", &symbol);
    while (symbol != ' ' && symbol != '\n' && symbol != '\r') {   
        //printf("SYMBOL %c INDEX %d\n", symbol, get_index(symbol));
        if (!node->children[get_index(symbol)]) {
            struct Node *node_new = malloc(sizeof(struct Node));
            *node_new = Node_init(LETTERS_NUMBER);
            node_new->parent = node;
            node_new->index = get_index(symbol);
            node->children[get_index(symbol)] = node_new;
        }
        //printf("-\n");
        node = node->children[get_index(symbol)];
        node->count++;
        scanf("%c", &symbol);
    }
    if (node->ends) {
        while (node) {
            node->count--;
            node = node->parent;
        }
        return;
    }
    node->ends++;
}

void Tree_delete(struct Tree *tree) {
    //printf("DELETE\n");
    struct Node *node = tree->root;
    char symbol = ' ';
    while (symbol == ' ' || symbol == '\n')
        scanf("%c", &symbol);
    while (symbol != ' ' && symbol != '\n' && symbol != '\r') {
        if (symbol == ' ' || symbol == '\n') {
            node->ends--;
            break;
        }
        node = node->children[get_index(symbol)];
        node->count--;
        if (!node->count) {
            node->parent->children[node->index] = 0;
            Tree_destructor_recursive(node);
            break;
        }
        scanf("%c", &symbol);
    }
    while (symbol != ' ' && symbol != '\n' && symbol != '\r')
        scanf("%c", &symbol);
}

int Tree_prefix(struct Tree *tree) {
    //printf("PREFIX\n");
    struct Node *node = tree->root;
    char symbol = ' ';
    int count = 0;
    while (symbol == ' ' || symbol == '\n')
        scanf("%c", &symbol);
    while (symbol != ' ' && symbol != '\n' && symbol != '\r') {
        //printf("SYMBOL %c INDEX %d\n", symbol, get_index(symbol));
        if (symbol == ' ' || symbol == '\n')
            break;
        node = node->children[get_index(symbol)];
        if (!node)
            break;
        scanf("%c", &symbol);
    }
    while (symbol != ' ' && symbol != '\n' && symbol != '\r')
        scanf("%c", &symbol);
    if (node)
        count = node->count;
    return count;
}

int main() {
    struct Tree tree = Tree_init();

    int n;
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        char command[10];
        scanf("%s", command);

        //printf("COMMAND %s\n", command);
        if (!strcmp(command, "INSERT")) {
            Tree_insert(&tree);
        }
        if (!strcmp(command, "DELETE")) {
            Tree_delete(&tree);
        }
        if (!strcmp(command, "PREFIX")) {
            printf("%d\n", Tree_prefix(&tree));
        }
    }

    Tree_destructor(&tree);
}