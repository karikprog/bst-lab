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

typedef struct {
    Node** stack;
    int capacity;
    int top;
} Iterator;

int bstInsert(BST* tree, int value);
BST* initTree();
Node* search(Node* node, int value);
void bstFree(BST** treeRef);
bool bstContains(BST* tree, int value);
Iterator* iteratorInit(const BST* tree);
bool iteratorHasNext(const Iterator* iter);
bool iteratorNext(Iterator* iter, int* result);
void iteratorFree(Iterator* iter);
bool bstMax(const BST* tree, int* result);
bool bstMin(const BST* tree, int* result);
int bstSize(const BST* tree);
int bstHeight(const BST* tree);
bool bstIsValid(const BST* tree);
bool bstKthMin(const BST* tree, int k, int* result);
int* bstInorder(const BST* tree);
int* bstPreorder(const BST* tree);
int* bstPostorder(const BST* tree);
BST* bstMerge(const BST* tree1, const BST* tree2);
