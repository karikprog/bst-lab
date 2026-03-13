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

void testIteratorInitNullTree()
{
    Iterator* iter = iteratorInit(NULL);
    checkPtrNull("iteratorInit NULL tree", iter);
}

void testIteratorEmptyTree()
{
    BST* tree = initTree();

    Iterator* iter = iteratorInit(tree);

    checkPtrNotNull("iteratorInit empty tree", iter);
    checkBool("hasNext on empty tree", false, iteratorHasNext(iter));

    iteratorFree(iter);
    bstFree(&tree);
}

void testIteratorSingleElement()
{
    BST* tree = initTree();
    bstInsert(tree, 10);

    Iterator* iter = iteratorInit(tree);

    checkBool("hasNext single element", true, iteratorHasNext(iter));

    int value = 0;
    bool ok = iteratorNext(iter, &value);

    checkBool("iteratorNext success", true, ok);
    checkInt("iteratorNext value", 10, value);

    checkBool("hasNext after last element", false, iteratorHasNext(iter));

    iteratorFree(iter);
    bstFree(&tree);
}

void testIteratorInorderTraversal()
{
    BST* tree = initTree();

    bstInsert(tree, 7);
    bstInsert(tree, 3);
    bstInsert(tree, 9);
    bstInsert(tree, 1);
    bstInsert(tree, 5);
    bstInsert(tree, 10);
    bstInsert(tree, 4);

    Iterator* iter = iteratorInit(tree);

    int expected[] = { 1, 3, 4, 5, 7, 9, 10 };
    int value = 0;

    for (int i = 0; i < 7; i++) {
        bool ok = iteratorNext(iter, &value);
        checkBool("iteratorNext inorder ok", true, ok);
        checkInt("iteratorNext inorder value", expected[i], value);
    }

    checkBool("hasNext after traversal", false, iteratorHasNext(iter));

    iteratorFree(iter);
    bstFree(&tree);
}

void testIteratorNextWhenEmpty()
{
    BST* tree = initTree();
    bstInsert(tree, 5);

    Iterator* iter = iteratorInit(tree);

    int value;

    iteratorNext(iter, &value);

    bool ok = iteratorNext(iter, &value);
    checkBool("iteratorNext on empty iterator", false, ok);

    iteratorFree(iter);
    bstFree(&tree);
}

void testIteratorFreeNull()
{
    iteratorFree(NULL);
    checkBool("iteratorFree NULL safe", true, true);
}

void testIteratorRightSubtree()
{
    BST* tree = initTree();

    bstInsert(tree, 5);
    bstInsert(tree, 3);
    bstInsert(tree, 7);
    bstInsert(tree, 6);

    Iterator* iter = iteratorInit(tree);

    int expected[] = { 3, 5, 6, 7 };
    int value;

    for (int i = 0; i < 4; i++) {
        bool ok = iteratorNext(iter, &value);
        checkBool("iteratorNext right subtree ok", true, ok);
        checkInt("iteratorNext right subtree value", expected[i], value);
    }

    checkBool("hasNext after right subtree traversal", false, iteratorHasNext(iter));
    iteratorFree(iter);
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

void testIsValidSimple()
{
    BST* tree = initTree();
    BST* treeEmpty = initTree();

    checkBool("check empty tree", true, bstIsValid(treeEmpty));
    checkBool("check null", false, bstIsValid(NULL));

    bstInsert(tree, 20);
    bstInsert(tree, 18);
    bstInsert(tree, 10);
    bstInsert(tree, 19);
    bstInsert(tree, 30);
    bstInsert(tree, 25);
    bstInsert(tree, 50);

    checkBool("check valide tree", true, bstIsValid(tree));

    tree->root->left->left->value = 100;
    tree->root->right->right->value = 5;

    checkBool("check invalid tree", false, bstIsValid(tree));
    bstFree(&tree);
}

void testIsValidAdvanced()
{
    BST* tree = initTree();
    bstInsert(tree, 50);
    bstInsert(tree, 30);
    bstInsert(tree, 70);
    bstInsert(tree, 60);
    bstInsert(tree, 80);

    checkBool("Advanced: tree is valid", true, bstIsValid(tree));

    tree->root->right->left->value = 20;

    checkBool("Advanced: global violation detected", false, bstIsValid(tree));

    bstFree(&tree);
}

//// Run tests
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

void testKthMin()
{
    BST* tree = initTree();

    bstInsert(tree, 20);
    bstInsert(tree, 18);
    bstInsert(tree, 10);
    bstInsert(tree, 19);
    bstInsert(tree, 30);
    bstInsert(tree, 25);
    bstInsert(tree, 50);

    int val;
    checkBool("find k-th minimum element error", true, bstKthMin(tree, 5, &val));
    checkBool("checking thats k is positive", false, bstKthMin(tree, -2, &val));
    checkBool("checking thats k more tree size", false, bstKthMin(tree, 10, &val));
    checkBool("checking that tree is NULL", false, bstKthMin(NULL, 3, &val));

    checkInt("invalid k-th minimum value", 25, val);
}

void testMergeNullTree()
{
    BST* tree1 = initTree();
    BST* tree2 = NULL;
    BST* newTree = bstMerge(tree1, tree2);
    checkPtrNull("attempt to merge null tree", newTree);
    bstFree(&tree1);
}

void testMergeEmptyTree()
{
    BST* tree1 = initTree();
    bstInsert(tree1, 3);
    bstInsert(tree1, 1);
    bstInsert(tree1, 2);
    bstInsert(tree1, 10);
    bstInsert(tree1, 7);
    BST* tree2 = initTree();
    BST* newTree = bstMerge(tree1, tree2);
    checkBool("contains 3", true, bstContains(newTree, 3));
    checkBool("contains 7", true, bstContains(newTree, 7));
    checkBool("contains 1", true, bstContains(newTree, 1));
    checkBool("contains 10", true, bstContains(newTree, 10));
    checkBool("contains 2", true, bstContains(newTree, 2));
    checkInt("size of new tree", 5, newTree->size);
    bstFree(&tree1);
    bstFree(&tree2);
    bstFree(&newTree);
}

void testMergeTreeWithDublicate()
{
    BST* tree1 = initTree();
    bstInsert(tree1, 3);
    bstInsert(tree1, 1);
    bstInsert(tree1, 2);
    bstInsert(tree1, 10);
    bstInsert(tree1, 7);
    BST* tree2 = initTree();
    bstInsert(tree2, 3);
    bstInsert(tree2, 1);
    bstInsert(tree2, 5);
    bstInsert(tree2, 12);
    bstInsert(tree2, 8);
    BST* newTree = bstMerge(tree1, tree2);
    checkBool("contains 3", true, bstContains(newTree, 3));
    checkBool("contains 1", true, bstContains(newTree, 1));
    checkBool("contains 2", true, bstContains(newTree, 2));
    checkBool("contains 10", true, bstContains(newTree, 10));
    checkBool("contains 7", true, bstContains(newTree, 7));
    checkBool("contains 5", true, bstContains(newTree, 5));
    checkBool("contains 12", true, bstContains(newTree, 12));
    checkBool("contains 8", true, bstContains(newTree, 8));
    checkInt("size of new tree", 8, newTree->size);
    bstFree(&tree1);
    bstFree(&tree2);
    bstFree(&newTree);
}

// Runtest
int runTests()
{
    testInitTree();
    testInsertSingle();
    testInsertMultiple();
    testDuplicateInsert();
    testSearch();
    testIteratorInitNullTree();
    testIteratorEmptyTree();
    testIteratorSingleElement();
    testIteratorInorderTraversal();
    testIteratorNextWhenEmpty();
    testIteratorFreeNull();
    testIteratorRightSubtree();
    testEmptyTreeFunctions();
    testSingleNodeFunctions();
    testMultipleNodes();
    testLeftSkewed();
    testRightSkewed();
    testWithZero();
    testNegativeOnly();
    testNullTree();
    testDuplicateInsertExtended();
    testIsValidSimple();
    testIsValidAdvanced();
    testKthMin();
    testMergeNullTree();
    testMergeEmptyTree();
    testMergeTreeWithDublicate();

    fprintf(stderr,
        "\nTests passed: %d\nTests failed: %d\n",
        testsPassed, testsFailed);

    if (testsFailed == 0) {
        fprintf(stderr, "ALL TESTS PASSED\n");
    } else {
        return 1;
    }
    return 0;
}