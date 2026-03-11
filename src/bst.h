#pragma once
#include <stdbool.h>

typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct
{
    Node* root;
    int size;
} BST;

int bstInsert(BST* tree, int value);
BST* initTree();
Node* search(Node* node, int value);
void bstFree(BST** treeRef);
bool bstContains(BST* tree, int value);
<<<<<<< HEAD
bool bstMax(const BST* tree, int* result);
bool bstMin(const BST* tree, int* result);
int bstSize(const BST* tree);
int bstHeight(const BST* tree);
=======
bool bstIsValid(const BST* tree);
>>>>>>> 6640205 (add tree valid func)
