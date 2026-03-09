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

void testEmptyTreeFunctions()
{
    BST* tree = initTree();
    checkInt("bstMax on empty tree", 0, bstMax(tree));
    checkInt("bstMin on empty tree", 0, bstMin(tree));
    checkInt("bstSize on empty tree", 0, bstSize(tree));
    checkInt("bstHeight on empty tree", 0, bstHeight(tree));
    bstFree(&tree);
}

void testSingleNodeFunctions()
{
    BST* tree = initTree();
    bstInsert(tree, 42);
    checkInt("bstMax single node", 42, bstMax(tree));
    checkInt("bstMin single node", 42, bstMin(tree));
    checkInt("bstSize single node", 1, bstSize(tree));
    checkInt("bstHeight single node", 1, bstHeight(tree));
    bstFree(&tree);
}

void testMultipleNodes()
{
    BST* tree = initTree();
    bstInsert(tree, 10);
    bstInsert(tree, 5);
    bstInsert(tree, 15);
    bstInsert(tree, 2);
    bstInsert(tree, 7);
    bstInsert(tree, 12);
    bstInsert(tree, 20);

    checkInt("bstMax multiple", 20, bstMax(tree));
    checkInt("bstMin multiple", 2, bstMin(tree));
    checkInt("bstSize multiple", 7, bstSize(tree));
    checkInt("bstHeight multiple (balanced)", 3, bstHeight(tree));
    bstFree(&tree);
}

void testLeftSkewed()
{
    BST* tree = initTree();
    bstInsert(tree, 40);
    bstInsert(tree, 30);
    bstInsert(tree, 20);
    bstInsert(tree, 10);

    checkInt("bstMax left-skewed", 40, bstMax(tree));
    checkInt("bstMin left-skewed", 10, bstMin(tree));
    checkInt("bstSize left-skewed", 4, bstSize(tree));
    checkInt("bstHeight left-skewed", 4, bstHeight(tree));
    bstFree(&tree);
}

void testRightSkewed()
{
    BST* tree = initTree();
    bstInsert(tree, 10);
    bstInsert(tree, 20);
    bstInsert(tree, 30);
    bstInsert(tree, 40);

    checkInt("bstMax right-skewed", 40, bstMax(tree));
    checkInt("bstMin right-skewed", 10, bstMin(tree));
    checkInt("bstSize right-skewed", 4, bstSize(tree));
    checkInt("bstHeight right-skewed", 4, bstHeight(tree));
    bstFree(&tree);
}

void testWithZero()
{
    BST* tree = initTree();
    bstInsert(tree, 0);
    bstInsert(tree, -5);
    bstInsert(tree, 5);

    checkInt("bstMax with zero", 5, bstMax(tree));
    checkInt("bstMin with zero", -5, bstMin(tree));
    checkInt("bstSize with zero", 3, bstSize(tree));
    checkInt("bstHeight with zero", 2, bstHeight(tree));
    bstFree(&tree);
}

// Runtest
int runTests()
{
    testInitTree();
    testInsertSingle();
    testInsertMultiple();
    testDuplicateInsert();
    testSearch();
    testEmptyTreeFunctions();
    testSingleNodeFunctions();
    testMultipleNodes();
    testLeftSkewed();
    testRightSkewed();
    testWithZero();

    fprintf(stderr,
        "\nTests passed: %d\nTests failed: %d\n",
        testsPassed, testsFailed);

    if (testsFailed == 0) {
        fprintf(stderr, "ALL TESTS PASSED\n");
    }
    return 0;
}
