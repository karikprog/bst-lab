#include "bst.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

///// Assistens checks

int testsPassed = 0;
int testsFailed = 0;

void checkInt(const char* testName, int expected, int actual)
{
    if (expected != actual) {
        fprintf(stderr,
            "FAILED: %s | expected=%d actual=%d\n",
            testName, expected, actual);
        testsFailed++;
    } else {
        testsPassed++;
    }
}

void checkBool(const char* testName, bool expected, bool actual)
{
    if (expected != actual) {
        fprintf(stderr,
            "FAILED: %s | expected=%d actual=%d\n",
            testName, expected, actual);
        testsFailed++;
    } else {
        testsPassed++;
    }
}

void checkPtrNotNull(const char* testName, void* ptr)
{
    if (ptr == NULL) {
        fprintf(stderr,
            "FAILED: %s | pointer is NULL\n",
            testName);
        testsFailed++;
    } else {
        testsPassed++;
    }
}

void checkPtrNull(const char* testName, void* ptr)
{
    if (ptr != NULL) {
        fprintf(stderr,
            "FAILED: %s | pointer is NOT NULL\n",
            testName);
        testsFailed++;
    } else {
        testsPassed++;
    }
}

///// Tests

void testInitTree()
{
    BST* tree = initTree();
    checkPtrNotNull("initTree not null", tree);
    if (tree != NULL) {
        checkPtrNull("root is NULL after init", tree->root);
        checkInt("size is 0 after init", 0, tree->size);
    }
    bstFree(&tree);
}

void testInsertSingle()
{
    BST* tree = initTree();

    int res = bstInsert(tree, 10);
    checkInt("insert returns 0", 0, res);
    checkInt("size after one insert", 1, tree->size);
    checkBool("contains 10", true, bstContains(tree, 10));
    checkBool("contains 5 (false)", false, bstContains(tree, 5));

    bstFree(&tree);
}

void testInsertMultiple()
{
    BST* tree = initTree();

    bstInsert(tree, 10);
    bstInsert(tree, 5);
    bstInsert(tree, 15);
    bstInsert(tree, 3);
    bstInsert(tree, 7);

    checkInt("size after 5 inserts", 5, tree->size);

    checkBool("contains 3", true, bstContains(tree, 3));
    checkBool("contains 7", true, bstContains(tree, 7));
    checkBool("contains 15", true, bstContains(tree, 15));
    checkBool("contains 100", false, bstContains(tree, 100));

    bstFree(&tree);
}

void testDuplicateInsert()
{
    BST* tree = initTree();

    bstInsert(tree, 10);
    int res = bstInsert(tree, 10);

    checkInt("duplicate insert returns 0", 0, res);
    checkInt("size not increased after duplicate", 1, tree->size);

    bstFree(&tree);
}

void testSearch()
{
    BST* tree = initTree();

    bstInsert(tree, 20);
    bstInsert(tree, 10);
    bstInsert(tree, 30);

    Node* n1 = search(tree->root, 10);
    Node* n2 = search(tree->root, 100);

    checkPtrNotNull("search existing node", n1);
    checkPtrNull("search non-existing node", n2);

    bstFree(&tree);
}

//// Run tests

int runTests()
{
    testInitTree();
    testInsertSingle();
    testInsertMultiple();
    testDuplicateInsert();
    testSearch();

    fprintf(stderr,
        "\nTests passed: %d\nTests failed: %d\n",
        testsPassed, testsFailed);

    if (testsFailed == 0) {
        fprintf(stderr, "ALL TESTS PASSED\n");
    }
    return 0;
}
