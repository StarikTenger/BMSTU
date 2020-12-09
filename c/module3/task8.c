#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    long long key;
    long long count;
    long long rank;
    char *val;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
};

struct Node Node_init_empty() {
    struct Node node;
    node.key = 0;
    node.val = 0;
    node.count = 0;
    node.right = 0;
    node.left = 0;
    node.parent = 0;
    return node;
}

struct Node Node_init(long long key, char *val) {
    struct Node node;
    node.key = key;
    node.val = val; 
    node.count = 0;   
    node.right = 0;
    node.left = 0;
    node.parent = 0;
    return node;
}

void Node_destructor(struct Node *node) {
    free(node->val);
}

struct Tree {
    struct Node *root;
};

struct Tree Tree_init() {
    struct Tree tree;
    tree.root = 0;
    return tree;
}

void Tree_delete_subtree(struct Node *node) {
    if (!node)
        return;
    Tree_delete_subtree(node->left);
    Tree_delete_subtree(node->right);
    free(node);
}

void Tree_destructor(struct Tree *tree) {
    Tree_delete_subtree(tree->root);
}

void Tree_update_count(struct Node *node, long long delta) {
    while (node) {
        node->count += delta;
        node = node->parent;
    }
}

void Tree_insert(struct Tree *tree, struct Node element) {
    if (!tree->root) {
        tree->root = malloc(sizeof(struct Node));
        *tree->root = element;
        Tree_update_count(tree->root, 1);
        return;
    }
    for (struct Node *current_node = tree->root;;) {
        if (current_node->key == element.key) {
            *current_node = element;
            break;
        }
        if (element.key < current_node->key) {
            if (current_node->left) {
                current_node = current_node->left;
            }
            else {
                current_node->left = malloc(sizeof(struct Node));
                element.parent = current_node;
                *current_node->left = element;
                Tree_update_count(current_node->left, 1);
                break;
            }
        }
        if (element.key > current_node->key) {
            if (current_node->right) {
                current_node = current_node->right;
            }
            else {
                current_node->right = malloc(sizeof(struct Node));
                element.parent = current_node;
                *current_node->right = element;
                Tree_update_count(current_node->right, 1);
                break;
            }
        }
    }
}

struct Node *Tree_search(struct Tree *tree, long long key) {
    struct Node *node = tree->root;
    while (node) {
        if (node->key == key) {
            return node;
        }
        if (key < node->key)
            node = node->left;
        else if (key > node->key)
            node = node->right;
    }
    return node;
}

void Tree_delete_node(struct Tree *tree, struct Node *node) {
    // No children
    if (!node->left && !node->right) {
        if (node->parent) {
            if (node == node->parent->left)
                node->parent->left = 0;
            if (node == node->parent->right)
                node->parent->right = 0;
        } else tree->root = 0;
        Tree_update_count(node, -1);
        free(node);
        return;
    }
    // One child
    // Left
    if (node->left && !node->right) {
        if (node->parent) {
            if (node == node->parent->left)
                node->parent->left = node->left;
            if (node == node->parent->right)
                node->parent->right = node->left;
        } else tree->root = node->left;
        node->left->parent = node->parent;
        Tree_update_count(node, -1);
        free(node);
        return;
    }
    // Right
    if (!node->left && node->right) {
        if (node->parent) {
            if (node == node->parent->left)
                node->parent->left = node->right;
            if (node == node->parent->right)
                node->parent->right = node->right;
        } else tree->root = node->right;
        node->right->parent = node->parent;
        Tree_update_count(node, -1);            
        free(node);
        return;
    }
    // Two children
    struct Node *node_right = node->right;
    while (node_right->left)
        node_right = node_right->left;

    struct Node node_new = *node_right;
    
    Tree_delete_node(tree, node_right);

    node_new.left = node->left;
    node_new.right = node->right;
    node_new.parent = node->parent;
    node_new.count = node->count;
    *node = node_new;
}

void Tree_delete(struct Tree *tree, long long key) {
    struct Node *found_node = Tree_search(tree, key);
    if (!found_node)
        return;
    Tree_delete_node(tree, found_node);
}

long long Tree_node_rank(struct Node *node) {
    if (!node->parent) {
        node->rank = node->count - (node->right ? node->right->count : 0);
        return node->rank;
    }
    if (node == node->parent->right) {
        node->rank = node->parent->rank + (node->left ? node->left->count : 0) + 1;
        return node->rank;
    }
    node->rank = node->parent->rank - (node->right ? node->right->count : 0) - 1;
    return node->rank;
}

struct Node *Tree_search_by_rank(struct Tree *tree, long long rank) {
    struct Node *node = tree->root;
    while (node) {
        if (Tree_node_rank(node) == rank)
            break;
        if (Tree_node_rank(node) > rank)
            node = node->left;
        else
            node = node->right;
    }
    return node;
}

int main() {
    struct Tree tree = Tree_init();    

    long long n;
    scanf("%lld", &n);
    char **strings = malloc(sizeof(char*) * n);
    long long strings_number = 0;

    for (long long i = 0; i < n; i++) {
        char command[10];
        scanf("%s", command);

        if (!strcmp(command, "INSERT")) {
            long long key;
            char *val = malloc(sizeof(char) * 10);
            scanf("%lld %s", &key, val);
            strings[strings_number++] = val;
            Tree_insert(&tree, Node_init(key, val));
        }
        if (!strcmp(command, "LOOKUP")) {
            long long key;
            scanf("%lld", &key);
            printf("%s\n", Tree_search(&tree, key)->val);
        }
        if (!strcmp(command, "DELETE")) {
            long long key;
            scanf("%lld", &key);
            Tree_delete(&tree, key);
        }
        if (!strcmp(command, "SEARCH")) {
            long long rank;
            scanf("%lld", &rank);
            printf("%s\n", Tree_search_by_rank(&tree, rank+1)->val);
            
        }
    }

    for (long long i = 0; i < strings_number; i++)
       free(strings[i]);
    free(strings);
    Tree_destructor(&tree);
}