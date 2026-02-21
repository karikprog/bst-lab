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
void bstFree(BST* tree);
bool bstContains(BST* tree, int value);
