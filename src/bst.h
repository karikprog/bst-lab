#pragma once
#include <stdbool.h>

typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct {
    Node* root;
    int size;
} BST;

int bstInsert(BST* tree, int value);
BST* initTree();
Node* search(Node* node, int value);
void bstFree(BST** treeRef);
bool bstContains(BST* tree, int value);
int bstInorder(const BST* tree);
int bstPreorder(const BST* tree);
int bstPostorder(const BST* tree);
