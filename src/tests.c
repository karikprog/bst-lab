#include "bst.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct BST {
    Node* root;
    int size;
} BST;

typedef struct Iterator {
    Node** stack;
    int capacity;
    int top;
} Iterator;

typedef struct {
    unsigned passed;
    unsigned faild;
} TestContext;

void checkInt(TestContext* context, char* testName, int expected, int actual)
{
    if (expected != actual) {
        fprintf(stderr,
            "FAILED: %s | expected=%d actual=%d\n",
            testName, expected, actual);
        context->faild++;
    } else {
        context->passed++;
    }
}

void checkBool(TestContext* context, char* testName, bool expected, bool actual)
{
    if (expected != actual) {
        fprintf(stderr,
            "FAILED: %s | expected=%d actual=%d\n",
            testName, expected, actual);
        context->faild++;
    } else {
        context->passed++;
    }
}

void checkPtrNotNull(TestContext* context, char* testName, void* ptr)
{
    if (ptr == NULL) {
        fprintf(stderr,
            "FAILED: %s | pointer is NULL\n",
            testName);
        context->faild++;
    } else {
        context->passed++;
    }
}

void checkPtrNull(TestContext* context, char* testName, void* ptr)
{
    if (ptr != NULL) {
        fprintf(stderr,
            "FAILED: %s | pointer is NOT NULL\n",
            testName);
        context->faild++;
    } else {
        context->passed++;
    }
}

///// Tests
void testInitTree(TestContext* context)
{
    BST* tree = initTree();
    checkPtrNotNull(context, "initTree not null", tree);
    if (tree != NULL) {
        checkPtrNull(context, "root is NULL after init", tree->root);
        checkInt(context, "size is 0 after init", 0, tree->size);
    }
    bstFree(&tree);
}

void testInsertSingle(TestContext* context)
{
    BST* tree = initTree();

    int res = bstInsert(tree, 10);
    checkInt(context, "insert returns 0", 0, res);
    checkInt(context, "size after one insert", 1, tree->size);
    checkBool(context, "contains 10", true, bstContains(tree, 10));
    checkBool(context, "contains 5 (false)", false, bstContains(tree, 5));

    bstFree(&tree);
}

void testInsertMultiple(TestContext* context)
{
    BST* tree = initTree();

    bstInsert(tree, 10);
    bstInsert(tree, 5);
    bstInsert(tree, 15);
    bstInsert(tree, 3);
    bstInsert(tree, 7);

    checkInt(context, "size after 5 inserts", 5, tree->size);

    checkBool(context, "contains 3", true, bstContains(tree, 3));
    checkBool(context, "contains 7", true, bstContains(tree, 7));
    checkBool(context, "contains 15", true, bstContains(tree, 15));
    checkBool(context, "contains 100", false, bstContains(tree, 100));

    bstFree(&tree);
}

void testDuplicateInsert(TestContext* context)
{
    BST* tree = initTree();

    bstInsert(tree, 10);
    int res = bstInsert(tree, 10);

    checkInt(context, "duplicate insert returns 0", 0, res);
    checkInt(context, "size not increased after duplicate", 1, tree->size);

    bstFree(&tree);
}

//// Run tests
void testIteratorInitNullTree(TestContext* context)
{
    Iterator* iter = iteratorInit(NULL);
    checkPtrNull(context, "iteratorInit NULL tree", iter);
}

void testIteratorEmptyTree(TestContext* context)
{
    BST* tree = initTree();

    Iterator* iter = iteratorInit(tree);

    checkPtrNotNull(context, "iteratorInit empty tree", iter);
    checkBool(context, "hasNext on empty tree", false, iteratorHasNext(iter));

    iteratorFree(iter);
    bstFree(&tree);
}

void testIteratorSingleElement(TestContext* context)
{
    BST* tree = initTree();
    bstInsert(tree, 10);

    Iterator* iter = iteratorInit(tree);

    checkBool(context, "hasNext single element", true, iteratorHasNext(iter));

    int value = 0;
    bool ok = iteratorNext(iter, &value);

    checkBool(context, "iteratorNext success", true, ok);
    checkInt(context, "iteratorNext value", 10, value);

    checkBool(context, "hasNext after last element", false, iteratorHasNext(iter));

    iteratorFree(iter);
    bstFree(&tree);
}

void testIteratorInorderTraversal(TestContext* context)
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
        checkBool(context, "iteratorNext inorder ok", true, ok);
        checkInt(context, "iteratorNext inorder value", expected[i], value);
    }

    checkBool(context, "hasNext after traversal", false, iteratorHasNext(iter));

    iteratorFree(iter);
    bstFree(&tree);
}

void testIteratorNextWhenEmpty(TestContext* context)
{
    BST* tree = initTree();
    bstInsert(tree, 5);

    Iterator* iter = iteratorInit(tree);

    int value;

    iteratorNext(iter, &value);

    bool ok = iteratorNext(iter, &value);
    checkBool(context, "iteratorNext on empty iterator", false, ok);

    iteratorFree(iter);
    bstFree(&tree);
}

void testIteratorFreeNull(TestContext* context)
{
    iteratorFree(NULL);
    checkBool(context, "iteratorFree NULL safe", true, true);
}

void testIteratorRightSubtree(TestContext* context)
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
        checkBool(context, "iteratorNext right subtree ok", true, ok);
        checkInt(context, "iteratorNext right subtree value", expected[i], value);
    }

    checkBool(context, "hasNext after right subtree traversal", false, iteratorHasNext(iter));
    iteratorFree(iter);
}

void testEmptyTreeFunctions(TestContext* context)
{
    BST* tree = initTree();
    int result;
    bool success;
    success = bstMax(tree, &result);
    checkBool(context, "bstMax on empty tree returns false", false, success);

    success = bstMin(tree, &result);
    checkBool(context, "bstMin on empty tree returns false", false, success);

    checkInt(context, "bstSize on empty tree", 0, bstSize(tree));
    checkInt(context, "bstHeight on empty tree", 0, bstHeight(tree));

    bstFree(&tree);
}

void testIsValidSimple(TestContext* context)
{
    BST* tree = initTree();
    BST* treeEmpty = initTree();

    checkBool(context, "check empty tree", true, bstIsValid(treeEmpty));
    checkBool(context, "check null", false, bstIsValid(NULL));

    bstInsert(tree, 20);
    bstInsert(tree, 18);
    bstInsert(tree, 10);
    bstInsert(tree, 19);
    bstInsert(tree, 30);
    bstInsert(tree, 25);
    bstInsert(tree, 50);

    checkBool(context, "check valide tree", true, bstIsValid(tree));

    tree->root->left->left->value = 100;
    tree->root->right->right->value = 5;

    checkBool(context, "check invalid tree", false, bstIsValid(tree));
    bstFree(&tree);
}

void testIsValidAdvanced(TestContext* context)
{
    BST* tree = initTree();
    bstInsert(tree, 50);
    bstInsert(tree, 30);
    bstInsert(tree, 70);
    bstInsert(tree, 60);
    bstInsert(tree, 80);

    checkBool(context, "Advanced: tree is valid", true, bstIsValid(tree));

    tree->root->right->left->value = 20;

    checkBool(context, "Advanced: global violation detected", false, bstIsValid(tree));

    bstFree(&tree);
}

//// Run tests
void testSingleNodeFunctions(TestContext* context)
{
    BST* tree = initTree();
    bstInsert(tree, 42);
    int result;
    bool success;

    success = bstMax(tree, &result);
    checkBool(context, "bstMax single node returns true", true, success);
    checkInt(context, "bstMax single node value", 42, result);

    success = bstMin(tree, &result);
    checkBool(context, "bstMin single node returns true", true, success);
    checkInt(context, "bstMin single node value", 42, result);

    checkInt(context, "bstSize single node", 1, bstSize(tree));
    checkInt(context, "bstHeight single node", 1, bstHeight(tree));

    bstFree(&tree);
}

void testMultipleNodes(TestContext* context)
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
    checkBool(context, "bstMax multiple returns true", true, success);
    checkInt(context, "bstMax multiple", 20, result);

    success = bstMin(tree, &result);
    checkBool(context, "bstMin multiple returns true", true, success);
    checkInt(context, "bstMin multiple", 2, result);

    checkInt(context, "bstSize multiple", 7, bstSize(tree));
    checkInt(context, "bstHeight multiple (balanced)", 3, bstHeight(tree));

    bstFree(&tree);
}

void testLeftSkewed(TestContext* context)
{
    BST* tree = initTree();
    bstInsert(tree, 40);
    bstInsert(tree, 30);
    bstInsert(tree, 20);
    bstInsert(tree, 10);

    int result;
    bool success;

    success = bstMax(tree, &result);
    checkBool(context, "bstMax left-skewed returns true", true, success);
    checkInt(context, "bstMax left-skewed", 40, result);

    success = bstMin(tree, &result);
    checkBool(context, "bstMin left-skewed returns true", true, success);
    checkInt(context, "bstMin left-skewed", 10, result);

    checkInt(context, "bstSize left-skewed", 4, bstSize(tree));
    checkInt(context, "bstHeight left-skewed", 4, bstHeight(tree));

    bstFree(&tree);
}

void testRightSkewed(TestContext* context)
{
    BST* tree = initTree();
    bstInsert(tree, 10);
    bstInsert(tree, 20);
    bstInsert(tree, 30);
    bstInsert(tree, 40);

    int result;
    bool success;

    success = bstMax(tree, &result);
    checkBool(context, "bstMax right-skewed returns true", true, success);
    checkInt(context, "bstMax right-skewed", 40, result);

    success = bstMin(tree, &result);
    checkBool(context, "bstMin right-skewed returns true", true, success);
    checkInt(context, "bstMin right-skewed", 10, result);

    checkInt(context, "bstSize right-skewed", 4, bstSize(tree));
    checkInt(context, "bstHeight right-skewed", 4, bstHeight(tree));

    bstFree(&tree);
}

void testWithZero(TestContext* context)
{
    BST* tree = initTree();
    bstInsert(tree, 0);
    bstInsert(tree, -5);
    bstInsert(tree, 5);

    int result;
    bool success;

    success = bstMax(tree, &result);
    checkBool(context, "bstMax with zero returns true", true, success);
    checkInt(context, "bstMax with zero", 5, result);

    success = bstMin(tree, &result);
    checkBool(context, "bstMin with zero returns true", true, success);
    checkInt(context, "bstMin with zero", -5, result);

    checkInt(context, "bstSize with zero", 3, bstSize(tree));
    checkInt(context, "bstHeight with zero", 2, bstHeight(tree));

    bstFree(&tree);
}

void testNegativeOnly(TestContext* context)
{
    BST* tree = initTree();
    bstInsert(tree, -10);
    bstInsert(tree, -5);
    bstInsert(tree, -3);
    bstInsert(tree, -20);

    int result;
    bool success;

    success = bstMax(tree, &result);
    checkBool(context, "bstMax negative only returns true", true, success);
    checkInt(context, "bstMax negative only", -3, result);

    success = bstMin(tree, &result);
    checkBool(context, "bstMin negative only returns true", true, success);
    checkInt(context, "bstMin negative only", -20, result);

    checkInt(context, "bstSize negative only", 4, bstSize(tree));

    bstFree(&tree);
}

void testNullTree(TestContext* context)
{
    int result;
    bool success;

    success = bstMax(NULL, &result);
    checkBool(context, "bstMax with NULL returns false", false, success);

    success = bstMin(NULL, &result);
    checkBool(context, "bstMin with NULL returns false", false, success);

    checkInt(context, "bstSize with NULL", 0, bstSize(NULL));
    checkInt(context, "bstHeight with NULL", 0, bstHeight(NULL));
}

void testDuplicateInsertExtended(TestContext* context)
{
    BST* tree = initTree();

    bstInsert(tree, 10);
    bstInsert(tree, 5);
    bstInsert(tree, 15);

    int result;
    bool success;

    success = bstMax(tree, &result);
    checkBool(context, "bstMax before duplicate returns true", true, success);
    checkInt(context, "bstMax before duplicate", 15, result);

    success = bstMin(tree, &result);
    checkBool(context, "bstMin before duplicate returns true", true, success);
    checkInt(context, "bstMin before duplicate", 5, result);

    int res = bstInsert(tree, 10);
    checkInt(context, "duplicate insert returns 0", 0, res);
    checkInt(context, "size not increased after duplicate", 3, tree->size);

    success = bstMax(tree, &result);
    checkBool(context, "bstMax after duplicate returns true", true, success);
    checkInt(context, "bstMax after duplicate unchanged", 15, result);

    success = bstMin(tree, &result);
    checkBool(context, "bstMin after duplicate returns true", true, success);
    checkInt(context, "bstMin after duplicate unchanged", 5, result);

    bstFree(&tree);
}

void testDeleteNodeRightChild(TestContext* context)
{
    BST* tree = initTree();

    bstInsert(tree, 10);
    bstInsert(tree, 15);
    bstInsert(tree, 20);

    int sizeBefore = bstSize(tree);

    bstDelete(tree, 15);

    checkBool(context, "delete right child: contains 15", false, bstContains(tree, 15));
    checkBool(context, "delete right child: contains 20", true, bstContains(tree, 20));
    checkInt(context, "delete right child: size decreased", sizeBefore - 1, bstSize(tree));
    checkBool(context, "delete right child: tree valid", true, bstIsValid(tree));

    bstFree(&tree);
}

void testDeleteNodeLeftChild(TestContext* context)
{
    BST* tree = initTree();

    bstInsert(tree, 10);
    bstInsert(tree, 5);
    bstInsert(tree, 2);

    int sizeBefore = bstSize(tree);

    bstDelete(tree, 5);

    checkBool(context, "delete left child: contains 5", false, bstContains(tree, 5));
    checkBool(context, "delete left child: contains 2", true, bstContains(tree, 2));
    checkInt(context, "delete left child: size decreased", sizeBefore - 1, bstSize(tree));
    checkBool(context, "delete left child: tree valid", true, bstIsValid(tree));

    bstFree(&tree);
}

void testDeleteTwoChildren(TestContext* context)
{
    BST* tree = initTree();

    bstInsert(tree, 10);
    bstInsert(tree, 5);
    bstInsert(tree, 15);
    bstInsert(tree, 2);
    bstInsert(tree, 7);

    int sizeBefore = bstSize(tree);

    bstDelete(tree, 5);

    checkBool(context, "delete two children: contains 5", false, bstContains(tree, 5));
    checkBool(context, "delete two children: contains 2", true, bstContains(tree, 2));
    checkBool(context, "delete two children: contains 7", true, bstContains(tree, 7));
    checkInt(context, "delete two children: size decreased", sizeBefore - 1, bstSize(tree));
    checkBool(context, "delete two children: tree valid", true, bstIsValid(tree));

    bstFree(&tree);
}

void testDeleteNonExisting(TestContext* context)
{
    BST* tree = initTree();

    bstInsert(tree, 10);
    bstInsert(tree, 5);
    bstInsert(tree, 15);

    int sizeBefore = bstSize(tree);

    bstDelete(tree, 999);

    checkInt(context, "delete non-existing: size unchanged", sizeBefore, bstSize(tree));
    checkBool(context, "delete non-existing: tree valid", true, bstIsValid(tree));
    checkBool(context, "delete non-existing: contains 10", true, bstContains(tree, 10));
    checkBool(context, "delete non-existing: contains 5", true, bstContains(tree, 5));
    checkBool(context, "delete non-existing: contains 15", true, bstContains(tree, 15));

    bstFree(&tree);
}

void testDeleteRoot(TestContext* context)
{
    BST* tree = initTree();

    bstInsert(tree, 10);
    bstInsert(tree, 5);
    bstInsert(tree, 15);

    int sizeBefore = bstSize(tree);

    bstDelete(tree, 10);

    checkBool(context, "delete root: contains 10", false, bstContains(tree, 10));
    checkBool(context, "delete root: contains 5", true, bstContains(tree, 5));
    checkBool(context, "delete root: contains 15", true, bstContains(tree, 15));
    checkInt(context, "delete root: size decreased", sizeBefore - 1, bstSize(tree));
    checkBool(context, "delete root: tree valid", true, bstIsValid(tree));

    bstFree(&tree);
}

void testDeleteSuccessorWithRightChild(TestContext* context)
{
    BST* tree = initTree();

    bstInsert(tree, 10);
    bstInsert(tree, 5);
    bstInsert(tree, 20);
    bstInsert(tree, 15);
    bstInsert(tree, 17);

    bstDelete(tree, 10);

    checkBool(context, "successor-right-child: contains 17", true, bstContains(tree, 17));
    checkBool(context, "successor-right-child: tree valid", true, bstIsValid(tree));
    checkInt(context, "successor-right-child: size", 4, bstSize(tree));

    bstFree(&tree);
}

void testDeleteKeepsSubtrees(TestContext* context)
{
    BST* tree = initTree();

    bstInsert(tree, 50);
    bstInsert(tree, 30);
    bstInsert(tree, 70);
    bstInsert(tree, 60);
    bstInsert(tree, 55);

    bstDelete(tree, 50);

    checkBool(context, "keep-subtree: contains 55", true, bstContains(tree, 55));
    checkBool(context, "keep-subtree: contains 60", true, bstContains(tree, 60));
    checkBool(context, "keep-subtree: tree valid", true, bstIsValid(tree));

    bstFree(&tree);
}

void testDeleteNodeWithSingleChildComplex(TestContext* context)
{
    BST* tree = initTree();

    bstInsert(tree, 10);
    bstInsert(tree, 20);
    bstInsert(tree, 15);

    bstDelete(tree, 20);

    checkBool(context, "single-child-complex: contains 15", true, bstContains(tree, 15));
    checkBool(context, "single-child-complex: tree valid", true, bstIsValid(tree));
    checkInt(context, "single-child-complex: size", 2, bstSize(tree));

    bstFree(&tree);
}

void testDeleteRootComplex(TestContext* context)
{
    BST* tree = initTree();

    bstInsert(tree, 40);
    bstInsert(tree, 20);
    bstInsert(tree, 60);
    bstInsert(tree, 10);
    bstInsert(tree, 30);
    bstInsert(tree, 50);
    bstInsert(tree, 70);

    bstDelete(tree, 40);

    checkBool(context, "delete-root-complex: tree valid", true, bstIsValid(tree));
    checkBool(context, "delete-root-complex: contains 50", true, bstContains(tree, 50));
    checkBool(context, "delete-root-complex: contains 20", true, bstContains(tree, 20));
    checkInt(context, "delete-root-complex: size", 6, bstSize(tree));

    bstFree(&tree);
}

void testDeleteMultipleSequential(TestContext* context)
{
    BST* tree = initTree();

    for (int i = 1; i <= 10; i++) {
        bstInsert(tree, i);
    }

    bstDelete(tree, 5);
    bstDelete(tree, 6);
    bstDelete(tree, 7);

    checkBool(context, "delete-multiple: contains 5", false, bstContains(tree, 5));
    checkBool(context, "delete-multiple: contains 6", false, bstContains(tree, 6));
    checkBool(context, "delete-multiple: contains 7", false, bstContains(tree, 7));

    checkBool(context, "delete-multiple: tree valid", true, bstIsValid(tree));
    checkInt(context, "delete-multiple: size", 7, bstSize(tree));

    bstFree(&tree);
}

void testIteratorAfterDelete(TestContext* context)
{
    BST* tree = initTree();

    bstInsert(tree, 5);
    bstInsert(tree, 3);
    bstInsert(tree, 7);
    bstInsert(tree, 6);

    bstDelete(tree, 5);

    Iterator* iter = iteratorInit(tree);

    int prev = -100000;
    int value;

    while (iteratorHasNext(iter)) {
        iteratorNext(iter, &value);
        checkBool(context, "iterator-after-delete sorted", value > prev, true);
        prev = value;
    }

    iteratorFree(iter);
    bstFree(&tree);
}
void testKthMin(TestContext* context)
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
    checkBool(context, "find k-th minimum element error", true, bstKthMin(tree, 5, &val));
    checkBool(context, "checking thats k is positive", false, bstKthMin(tree, -2, &val));
    checkBool(context, "checking thats k more tree size", false, bstKthMin(tree, 10, &val));
    checkBool(context, "checking that tree is NULL", false, bstKthMin(NULL, 3, &val));

    checkInt(context, "invalid k-th minimum value", 25, val);
}

void testDfsEmptyTree(TestContext* context)
{
    BST* tree = initTree();
    int* vertices = bstInorder(tree);
    checkPtrNull(context, "inOrder returns NULL if tree is empty", vertices);
    free(vertices);
    vertices = NULL;

    vertices = bstPreorder(tree);
    checkPtrNull(context, "preOrder returns -1 if tree is empty", vertices);
    free(vertices);
    vertices = NULL;

    vertices = bstPostorder(tree);
    checkPtrNull(context, "postOrder returns -1 if tree is empty", vertices);
    free(vertices);
    vertices = NULL;

    bstFree(&tree);
}

void testDfsOneNode(TestContext* context)
{
    BST* tree = initTree();
    bstInsert(tree, 10);

    int* vertices = bstInorder(tree);
    for (int i = 0; i < tree->size; i++) {
        checkInt(context, "vertices in Inprder", 10, vertices[i]);
    }
    free(vertices);
    vertices = NULL;

    vertices = bstPreorder(tree);
    for (int i = 0; i < tree->size; i++) {
        checkInt(context, "vertices in Preorder", 10, vertices[i]);
    }
    free(vertices);
    vertices = NULL;

    vertices = bstPostorder(tree);
    for (int i = 0; i < tree->size; i++) {
        checkInt(context, "vertices in Postorder", 10, vertices[i]);
    }
    free(vertices);
    vertices = NULL;

    bstFree(&tree);
}

void testDfsOnOnlyLeftSybtree(TestContext* context)
{
    BST* tree = initTree();
    bstInsert(tree, 10);
    bstInsert(tree, 9);
    bstInsert(tree, 8);
    bstInsert(tree, 7);

    int expVerticesForInorder[] = { 7, 8, 9, 10 };
    int* vertices = bstInorder(tree);
    for (int i = 0; i < tree->size; i++) {
        checkInt(context, "vertices in Inprder", expVerticesForInorder[i], vertices[i]);
    }
    free(vertices);
    vertices = NULL;

    int expVerticesForPreorder[] = { 10, 9, 8, 7 };
    vertices = bstPreorder(tree);
    for (int i = 0; i < tree->size; i++) {
        checkInt(context, "vertices in Preorder", expVerticesForPreorder[i], vertices[i]);
    }
    free(vertices);
    vertices = NULL;

    int expVerticesForPostorder[] = { 7, 8, 9, 10 };
    vertices = bstPostorder(tree);
    for (int i = 0; i < tree->size; i++) {
        checkInt(context, "vertices in Postorder", expVerticesForPostorder[i], vertices[i]);
    }
    free(vertices);
    vertices = NULL;

    bstFree(&tree);
}

void testDfsOnOnlyRightSybtree(TestContext* context)
{
    BST* tree = initTree();
    bstInsert(tree, 10);
    bstInsert(tree, 11);
    bstInsert(tree, 12);
    bstInsert(tree, 13);

    int expVerticesForInorder[] = { 10, 11, 12, 13 };
    int* vertices = bstInorder(tree);
    for (int i = 0; i < tree->size; i++) {
        checkInt(context, "vertices in Inprder", expVerticesForInorder[i], vertices[i]);
    }
    free(vertices);
    vertices = NULL;

    int expVerticesForPreorder[] = { 10, 11, 12, 13 };
    vertices = bstPreorder(tree);
    for (int i = 0; i < tree->size; i++) {
        checkInt(context, "vertices in Preorder", expVerticesForPreorder[i], vertices[i]);
    }
    free(vertices);
    vertices = NULL;

    int expVerticesForPostorder[] = { 13, 12, 11, 10 };
    vertices = bstPostorder(tree);
    for (int i = 0; i < tree->size; i++) {
        checkInt(context, "vertices in Postorder", expVerticesForPostorder[i], vertices[i]);
    }
    free(vertices);
    vertices = NULL;

    bstFree(&tree);
}

void testDfsOnNormalTree(TestContext* context)
{
    BST* tree = initTree();
    bstInsert(tree, 7);
    bstInsert(tree, 3);
    bstInsert(tree, 9);
    bstInsert(tree, 1);
    bstInsert(tree, 5);
    bstInsert(tree, 4);
    bstInsert(tree, 9);
    bstInsert(tree, 10);

    int expVerticesForInorder[] = { 1, 3, 4, 5, 7, 9, 10 };
    int* vertices = bstInorder(tree);
    for (int i = 0; i < tree->size; i++) {
        checkInt(context, "vertices in Inprder", expVerticesForInorder[i], vertices[i]);
    }
    free(vertices);
    vertices = NULL;

    int expVerticesForPreorder[] = { 7, 3, 1, 5, 4, 9, 10 };
    vertices = bstPreorder(tree);
    for (int i = 0; i < tree->size; i++) {
        checkInt(context, "vertices in Preorder", expVerticesForPreorder[i], vertices[i]);
    }
    free(vertices);
    vertices = NULL;

    int expVerticesForPostorder[] = { 1, 4, 5, 3, 10, 9, 7 };
    vertices = bstPostorder(tree);
    for (int i = 0; i < tree->size; i++) {
        checkInt(context, "vertices in Postorder", expVerticesForPostorder[i], vertices[i]);
    }
    free(vertices);
    vertices = NULL;

    bstFree(&tree);
}

void testMergeNullTree(TestContext* context)
{
    BST* tree1 = initTree();
    BST* tree2 = NULL;
    BST* newTree = bstMerge(tree1, tree2);
    checkPtrNull(context, "attempt to merge null tree", newTree);
    bstFree(&tree1);
}

void testMergeEmptyTree(TestContext* context)
{
    BST* tree1 = initTree();
    bstInsert(tree1, 3);
    bstInsert(tree1, 1);
    bstInsert(tree1, 2);
    bstInsert(tree1, 10);
    bstInsert(tree1, 7);
    BST* tree2 = initTree();
    BST* newTree = bstMerge(tree1, tree2);
    checkBool(context, "contains 3", true, bstContains(newTree, 3));
    checkBool(context, "contains 7", true, bstContains(newTree, 7));
    checkBool(context, "contains 1", true, bstContains(newTree, 1));
    checkBool(context, "contains 10", true, bstContains(newTree, 10));
    checkBool(context, "contains 2", true, bstContains(newTree, 2));
    checkInt(context, "size of new tree", 5, newTree->size);
    bstFree(&tree1);
    bstFree(&tree2);
    bstFree(&newTree);
}

void testMergeTreeWithDublicate(TestContext* context)
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
    checkBool(context, "contains 3", true, bstContains(newTree, 3));
    checkBool(context, "contains 1", true, bstContains(newTree, 1));
    checkBool(context, "contains 2", true, bstContains(newTree, 2));
    checkBool(context, "contains 10", true, bstContains(newTree, 10));
    checkBool(context, "contains 7", true, bstContains(newTree, 7));
    checkBool(context, "contains 5", true, bstContains(newTree, 5));
    checkBool(context, "contains 12", true, bstContains(newTree, 12));
    checkBool(context, "contains 8", true, bstContains(newTree, 8));
    checkInt(context, "size of new tree", 8, newTree->size);
    bstFree(&tree1);
    bstFree(&tree2);
    bstFree(&newTree);
}

// Runtest
int runTests()
{
    TestContext context = { 0, 0 };
    testInitTree(&context);
    testInsertSingle(&context);
    testInsertMultiple(&context);
    testDuplicateInsert(&context);
    testIteratorInitNullTree(&context);
    testIteratorEmptyTree(&context);
    testIteratorSingleElement(&context);
    testIteratorInorderTraversal(&context);
    testIteratorNextWhenEmpty(&context);
    testIteratorFreeNull(&context);
    testIteratorRightSubtree(&context);
    testEmptyTreeFunctions(&context);
    testSingleNodeFunctions(&context);
    testMultipleNodes(&context);
    testLeftSkewed(&context);
    testRightSkewed(&context);
    testWithZero(&context);
    testNegativeOnly(&context);
    testNullTree(&context);
    testDuplicateInsertExtended(&context);
    testIsValidSimple(&context);
    testIsValidAdvanced(&context);
    testDeleteNodeRightChild(&context);
    testDeleteNodeLeftChild(&context);
    testDeleteTwoChildren(&context);
    testDeleteNonExisting(&context);
    testDeleteRoot(&context);
    testDeleteSuccessorWithRightChild(&context);
    testDeleteKeepsSubtrees(&context);
    testDeleteNodeWithSingleChildComplex(&context);
    testDeleteRootComplex(&context);
    testDeleteMultipleSequential(&context);
    testIteratorAfterDelete(&context);
    testKthMin(&context);
    testDfsEmptyTree(&context);
    testDfsOneNode(&context);
    testDfsOnNormalTree(&context);
    testDfsOnOnlyLeftSybtree(&context);
    testDfsOnOnlyRightSybtree(&context);
    testMergeNullTree(&context);
    testMergeEmptyTree(&context);
    testMergeTreeWithDublicate(&context);
    fprintf(stderr,
        "\nTests passed: %d\nTests failed: %d\n",
        context.passed, context.faild);

    if (context.faild == 0) {
        fprintf(stderr, "ALL TESTS PASSED\n");
    } else {
        return 1;
    }
    return 0;
}