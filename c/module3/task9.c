#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    int key;
    int count;
    int rank;
    int height;
    int val;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
};

struct Node Node_init_empty() {
    struct Node node;
    node.key = 0;
    node.val = 0;
    node.count = 0;
    node.height = 1;
    node.right = 0;
    node.left = 0;
    node.parent = 0;
    return node;
}

struct Node Node_init(int key, int val) {
    struct Node node;
    node.key = key;
    node.val = val; 
    node.count = 0;   
    node.height = 1;
    node.right = 0;
    node.left = 0;
    node.parent = 0;
    return node;
}

struct Tree {
    struct Node *root;
};

void Tree_print(struct Node *node);

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

int Tree_height(struct Node *node) {
    return node ? node->height : 0;
}

int Tree_diff(struct Node *node) {
    return Tree_height(node->right) - Tree_height(node->left);
}

void Tree_fix_height(struct Node *node) {
    int height_left = Tree_height(node->left);
    int height_right = Tree_height(node->right);
    node->height = (height_left > height_right ? height_left : height_right) + 1;
}

struct Node *Tree_rotate_right(struct Node *node) {
    printf("ROTATE RIGHT %d\n", node->key);
	struct Node *child = node->left;
	node->left = child->right;
	child->right = node;
	Tree_fix_height(node);
	Tree_fix_height(child);
	return child;
}

struct Node *Tree_rotate_left(struct Node *node) {
    printf("ROTATE LEFT %d -> ", node->key);
	struct Node *child = node->right;
	node->right = child->left;
	child->left = node;
	Tree_fix_height(node);
	Tree_fix_height(child);
    printf("%d\n", child->key);
	return child;
}

struct Node *Tree_balance(struct Node *node) {
    Tree_fix_height(node);
    if (Tree_diff(node) == 2) {
        if (Tree_diff(node->right) < 0 )
			node->right = Tree_rotate_right(node->right);
		return Tree_rotate_left(node);
    }
    if (Tree_diff(node) == -2) {
        if (Tree_diff(node->left) < 0 )
			node->left = Tree_rotate_left(node->left);
		return Tree_rotate_right(node);
    }
    return node;
}

struct Node *Tree_insert_recursive(struct Node *node, struct Node element) {
    if (!node) {
        struct Node *node_new = malloc(sizeof(struct Node));
        *node_new = element;
        return node_new;
    }
    if (element.key < node->key)
        node->left = Tree_insert_recursive(node->left, element);
    else
        node->right = Tree_insert_recursive(node->right, element);
    return Tree_balance(node);
}

void Tree_print(struct Node *node) {
    if (node->left)
        Tree_print(node->left);
    if (node->right)
        Tree_print(node->right);
    for (int i = 0; i < 10 - node->height; i++)
        printf("    ");
    printf("KEY: %d ", node->key);
    printf("L: %d ", node->left ? node->left->key : -1);
    printf("R: %d ", node->right ? node->right->key : -1);
    printf("H: %d ", node->height);
    printf("D: %d\n", Tree_diff(node));
    
}

void Tree_insert(struct Tree *tree, struct Node element) {
    if (!tree->root) {
        tree->root = malloc(sizeof(struct Node));
        *tree->root = element;
        return;
    }
    tree->root = Tree_insert_recursive(tree->root, element);
}

struct Node *Tree_search(struct Tree *tree, int key) {
    
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

struct Node *Tree_find_min(struct Node *node) {
	return node->left ? Tree_find_min(node->left) : node;
}

struct Node *Tree_delete_min(struct Node *node) {
	if( node->left==0 )
		return node->right;
	node->left = Tree_delete_min(node->left);
	return Tree_balance(node);
}

struct Node *Tree_delete_recursive(struct Node *node, int key) {
    if(!node) 
        return 0;
	if (key < node->key)
		node->left = Tree_delete_recursive(node->left, key);
	else if (key > node->key)
		node->right = Tree_delete_recursive(node->right, key);	
	else {
		struct Node *left = node->left;
		struct Node *right = node->right;
		free(node);
		if (!right) 
            return left;
		struct Node *min = Tree_find_min(right);
		min->right = Tree_delete_min(right);
		min->left = left;
		return Tree_balance(min);
	}
	return Tree_balance(node);
}

void Tree_delete(struct Tree *tree, int key) {
    tree->root = Tree_delete_recursive(tree->root, key);
}

int string_hash(char *str) {
    int module = 32423;
    int hash = 0;
    int len = strlen(str);
    for (int i = 0, k = 1; i < len; i++, k = (k * 256) % module) {
        hash = (hash + (int)str[i] * k) % module;
    }
    return hash;
}

int main() {
    struct Tree tree = Tree_init();    

    int n;
    scanf("%d", &n);

    char word[1000];
    int word_len = 0;

    char specs[7] = "+-*/()";
    // Type names
    const int CONST = 1;
    const int SPEC = 2;
    const int IDENT = 3;
    int type = 0;

    int last_ident = 0;

    int symbol_prev = 0;
    for (int i = 0; i < n + 1; i++) {
        char symbol;
        if (i < n)
            scanf("\n%c", &symbol);
        else
            symbol = ' ';
        
        //printf("%d : %c\n", i, symbol);
        int spec_number = -1;
        for (int i = 0; i < 6; i++) {
            if(symbol == specs[i]) {
                spec_number = i;
                break;
            }
        }
        // End of the word
        if (word_len && (symbol == ' ' || spec_number != -1)) {
            word[word_len++] = '\0';
            if (type == CONST) {
                printf("CONST %s\n", word);
            } 
            else {
                if (!Tree_search(&tree, string_hash(word)))
                    Tree_insert(&tree, Node_init(string_hash(word), last_ident++));
                printf("IDENT %d\n", Tree_search(&tree, string_hash(word))->val);
            }
            
            word_len = 0;
        }
        if (symbol != ' ') {
            if (spec_number != -1) {
                printf("SPEC %d\n", spec_number);
                symbol_prev = 0;
                continue;
            }
            word[word_len++] = symbol;
            if (!symbol_prev) { // Read starts
                if (symbol >= '0' && symbol <= '9')
                    type = CONST;
                else
                    type = IDENT;       
            }
            symbol_prev = 1;
        }
        else {
            symbol_prev = 0;
        }
        
    }

    Tree_destructor(&tree);
}