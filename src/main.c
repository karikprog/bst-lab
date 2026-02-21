#include "bst.h"
#include <stdbool.h>
#include <stdio.h>

// Primitive test
int main(void)
{
    BST* tree = InitTree();
    int err = bstInsert(tree, 10);
    if (err == -1) {
        printf("Memory allocation error\n");
    }
    err = bstInsert(tree, 14);
    if (err == -1) {
        printf("Memory allocation error\n");
    }
    err = bstInsert(tree, 112);
    if (err == -1) {
        printf("Memory allocation error\n");
    }
    err = bstInsert(tree, 3);
    if (err == -1) {
        printf("Memory allocation error\n");
    }
    printf("%d\n", bstContains(tree, 112));
    printf("%d\n", bstContains(tree, 3));
    printf("%d\n", bstContains(tree, 14));
    printf("%d\n", bstContains(tree, 99));
    bstFree(tree);
    return 0;
}