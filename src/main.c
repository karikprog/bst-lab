#include "bst.h"
#include "tests.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Primitive test
int main(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--test") == 0) {
            return runTests();
        }
    }

    BST* tree = initTree();
    int err = bstInsert(tree, 10);
    if (err == -1) {
        printf("Memory allocation error\n");
        bstFree(&tree);
        return 1;
    }
    err = bstInsert(tree, 14);
    if (err == -1) {
        printf("Memory allocation error\n");
        bstFree(&tree);
        return 1;
    }
    err = bstInsert(tree, 112);
    if (err == -1) {
        printf("Memory allocation error\n");
        bstFree(&tree);
        return 1;
    }
    err = bstInsert(tree, 3);
    if (err == -1) {
        printf("Memory allocation error\n");
        bstFree(&tree);
        return 1;
    }
    printf("%d\n", bstContains(tree, 112));
    printf("%d\n", bstContains(tree, 3));
    printf("%d\n", bstContains(tree, 14));
    printf("%d\n", bstContains(tree, 99));
    bstFree(&tree);
    return 0;
}