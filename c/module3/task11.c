#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TREE ////////////////////////////////////////////////////////////////

struct Node {
    long long key;
    long long count;
    long long rank;
    long long height;
    long long val;
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

struct Node Node_init(long long key, long long val) {
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

long long Tree_height(struct Node *node) {
    return node ? node->height : 0;
}

long long Tree_diff(struct Node *node) {
    return Tree_height(node->right) - Tree_height(node->left);
}

void Tree_fix_height(struct Node *node) {
    long long height_left = Tree_height(node->left);
    long long height_right = Tree_height(node->right);
    node->height = (height_left > height_right ? height_left : height_right) + 1;
}

struct Node *Tree_rotate_right(struct Node *node) {
	struct Node *child = node->left;
	node->left = child->right;
	child->right = node;
	Tree_fix_height(node);
	Tree_fix_height(child);
	return child;
}

struct Node *Tree_rotate_left(struct Node *node) {
	struct Node *child = node->right;
	node->right = child->left;
	child->left = node;
	Tree_fix_height(node);
	Tree_fix_height(child);
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
        if (Tree_diff(node->left) > 0 )
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

void Tree_insert(struct Tree *tree, struct Node element) {
    if (!tree->root) {
        tree->root = malloc(sizeof(struct Node));
        *tree->root = element;
        return;
    }
    tree->root = Tree_insert_recursive(tree->root, element);
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

struct Node *Tree_find_min(struct Node *node) {
	return node->left ? Tree_find_min(node->left) : node;
}

struct Node *Tree_delete_min(struct Node *node) {
	if( node->left==0 )
		return node->right;
	node->left = Tree_delete_min(node->left);
	return Tree_balance(node);
}

struct Node *Tree_delete_recursive(struct Node *node, long long key) {
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

void Tree_delete(struct Tree *tree, long long key) {
    tree->root = Tree_delete_recursive(tree->root, key);
}

// TABLE ///////////////////////////////////////////////////////////////

struct Hash_table {
    struct Tree *table;
    long long size;
};

struct Hash_table Hash_table_init(long long size) {
    struct Hash_table table;
    table.size = size;
    table.table = malloc(sizeof(struct Tree) * size);
    for (long long i = 0; i < size; i++) {
        table.table[i] = Tree_init();
    }
    return table;
}

void Hash_table_destructor(struct Hash_table *table) {
    for (long long i = 0; i < table->size; i++) {
        Tree_destructor(&table->table[i]);
    }
    free(table->table);
}

void Hash_table_insert(struct Hash_table *table, long long key, long long val) {
    struct Tree *tree = &table->table[key % table->size];
    struct Node *node = Tree_search(tree, key);
    if (node)
        node->val = val;
    else
        Tree_insert(tree, Node_init(key, val));
}

struct Node *Hash_table_search(struct Hash_table *table, long long key) {
    struct Tree *tree = &table->table[key % table->size];
    struct Node *node = Tree_search(tree, key);
    if (!node)
        Tree_insert(tree, Node_init(key, 0));
    return Tree_search(tree, key);
}

long long main() {
    long long n;
    scanf("%lld", &n);

    struct Hash_table table = Hash_table_init(2017);

    long long last = 0;
    long long  count = 0;
    long long sum = 0;
    for (long long i = 0; i < n; i++) {
        long long num;
        scanf("%lld", &num);        
        sum ^= num;
        //printf("%d : %d\n", sum, Hash_table_search(&table, sum)->val);
        if(sum)
            count += Hash_table_search(&table, sum)->val++;
        else
            count += ++Hash_table_search(&table, sum)->val;
        
    }
    printf("%lld", count);

    Hash_table_destructor(&table);
}