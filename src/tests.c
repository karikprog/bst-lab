#include "bst.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
    int result;
    bool success;

    success = bstMax(tree, &result);
    checkBool("bstMax on empty tree returns false", false, success);

    success = bstMin(tree, &result);
    checkBool("bstMin on empty tree returns false", false, success);

    checkInt("bstSize on empty tree", 0, bstSize(tree));
    checkInt("bstHeight on empty tree", 0, bstHeight(tree));

    bstFree(&tree);
}

void testSingleNodeFunctions()
{
    BST* tree = initTree();
    bstInsert(tree, 42);
    int result;
    bool success;

    success = bstMax(tree, &result);
    checkBool("bstMax single node returns true", true, success);
    checkInt("bstMax single node value", 42, result);

    success = bstMin(tree, &result);
    checkBool("bstMin single node returns true", true, success);
    checkInt("bstMin single node value", 42, result);

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

    int result;
    bool success;

    success = bstMax(tree, &result);
    checkBool("bstMax multiple returns true", true, success);
    checkInt("bstMax multiple", 20, result);

    success = bstMin(tree, &result);
    checkBool("bstMin multiple returns true", true, success);
    checkInt("bstMin multiple", 2, result);

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

    int result;
    bool success;

    success = bstMax(tree, &result);
    checkBool("bstMax left-skewed returns true", true, success);
    checkInt("bstMax left-skewed", 40, result);

    success = bstMin(tree, &result);
    checkBool("bstMin left-skewed returns true", true, success);
    checkInt("bstMin left-skewed", 10, result);

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

    int result;
    bool success;

    success = bstMax(tree, &result);
    checkBool("bstMax right-skewed returns true", true, success);
    checkInt("bstMax right-skewed", 40, result);

    success = bstMin(tree, &result);
    checkBool("bstMin right-skewed returns true", true, success);
    checkInt("bstMin right-skewed", 10, result);

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

    int result;
    bool success;

    success = bstMax(tree, &result);
    checkBool("bstMax with zero returns true", true, success);
    checkInt("bstMax with zero", 5, result);

    success = bstMin(tree, &result);
    checkBool("bstMin with zero returns true", true, success);
    checkInt("bstMin with zero", -5, result);

    checkInt("bstSize with zero", 3, bstSize(tree));
    checkInt("bstHeight with zero", 2, bstHeight(tree));

    bstFree(&tree);
}

void testNegativeOnly()
{
    BST* tree = initTree();
    bstInsert(tree, -10);
    bstInsert(tree, -5);
    bstInsert(tree, -3);
    bstInsert(tree, -20);

    int result;
    bool success;

    success = bstMax(tree, &result);
    checkBool("bstMax negative only returns true", true, success);
    checkInt("bstMax negative only", -3, result);

    success = bstMin(tree, &result);
    checkBool("bstMin negative only returns true", true, success);
    checkInt("bstMin negative only", -20, result);

    checkInt("bstSize negative only", 4, bstSize(tree));

    bstFree(&tree);
}

void testNullTree()
{
    int result;
    bool success;

    success = bstMax(NULL, &result);
    checkBool("bstMax with NULL returns false", false, success);

    success = bstMin(NULL, &result);
    checkBool("bstMin with NULL returns false", false, success);

    checkInt("bstSize with NULL", 0, bstSize(NULL));
    checkInt("bstHeight with NULL", 0, bstHeight(NULL));
}

void testDuplicateInsertExtended()
{
    BST* tree = initTree();

    bstInsert(tree, 10);
    bstInsert(tree, 5);
    bstInsert(tree, 15);

    int result;
    bool success;

    success = bstMax(tree, &result);
    checkBool("bstMax before duplicate returns true", true, success);
    checkInt("bstMax before duplicate", 15, result);

    success = bstMin(tree, &result);
    checkBool("bstMin before duplicate returns true", true, success);
    checkInt("bstMin before duplicate", 5, result);

    int res = bstInsert(tree, 10);
    checkInt("duplicate insert returns 0", 0, res);
    checkInt("size not increased after duplicate", 3, tree->size);

    success = bstMax(tree, &result);
    checkBool("bstMax after duplicate returns true", true, success);
    checkInt("bstMax after duplicate unchanged", 15, result);

    success = bstMin(tree, &result);
    checkBool("bstMin after duplicate returns true", true, success);
    checkInt("bstMin after duplicate unchanged", 5, result);

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
    testNegativeOnly();
    testNullTree();
    testDuplicateInsertExtended();

    fprintf(stderr,
        "\nTests passed: %d\nTests failed: %d\n",
        testsPassed, testsFailed);

    if (testsFailed == 0) {
        fprintf(stderr, "ALL TESTS PASSED\n");
    }
    return 0;
}