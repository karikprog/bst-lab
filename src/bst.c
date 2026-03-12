#include "bst.h"
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

BST* initTree()
{
    BST* tree = (BST*)malloc(sizeof(BST));
    if (tree == NULL) {
        return NULL;
    }
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

void cleanTree(Node* root)
{
    if (root == NULL) {
        return;
    }
    cleanTree(root->left);
    cleanTree(root->right);

    free(root);
}

void bstFree(BST** treeRef)
{
    BST* t = *treeRef;
    if (treeRef == NULL || *treeRef == NULL) {
        return;
    }

    cleanTree(t->root);
    free(t);
    *treeRef = NULL;
}

Node* search(Node* node, int value)
{
    if (node == NULL || node->value == value) {
        return node;
    }
    if (node->value > value) {
        return search(node->left, value);
    }
    return search(node->right, value);
}

bool bstContains(BST* tree, int value)
{
    if (tree == NULL) {
        return false;
    }
    if (search(tree->root, value) != NULL) {
        return true;
    }
    return false;
}

int bstInsert(BST* tree, int value)
{
    if (tree == NULL) {
        return -1;
    }

    Node* temp = (Node*)malloc(sizeof(Node));
    if (temp == NULL) {
        return -1;
    }
    temp->value = value;
    temp->left = NULL;
    temp->right = NULL;
    if (tree->size == 0) {
        tree->root = temp;
        tree->size++;
        return 0;
    }
    Node* node = tree->root;

    while (1) {
        if (node->value == value) {
            free(temp);
            return 0;
        }
        if (node->value > value) {
            if (node->left == NULL) {
                node->left = temp;
                tree->size++;
                return 0;
            } else {
                node = node->left;
            }
        } else {
            if (node->right == NULL) {
                node->right = temp;
                tree->size++;
                return 0;
            } else {
                node = node->right;
            }
        }
    }
}

static void pushLeft(Iterator* iter, Node* node)
{
    if (iter == NULL || node == NULL) {
        return;
    }

    while (node != NULL) {
        iter->stack[iter->top++] = node;
        node = node->left;
    }
}

Iterator* iteratorInit(const BST* tree)
{
    if (tree == NULL) {
        return NULL;
    }

    Iterator* iter = malloc(sizeof(Iterator));
    if (iter == NULL) {
        return NULL;
    }
    iter->capacity = tree->size;
    iter->top = 0;
    iter->stack = malloc(sizeof(Node*) * iter->capacity);
    if (iter->stack == NULL) {
        free(iter);
        return NULL;
    }

    pushLeft(iter, tree->root);
    return iter;
}

bool iteratorHasNext(const Iterator* iter)
{
    if (iter == NULL || iter->top == 0) {
        return false;
    }
    return true;
}

bool iteratorNext(Iterator* iter, int* result)
{
    if (!iteratorHasNext(iter)) {
        return false;
    }

    const Node* node = iter->stack[--iter->top];
    if (node->right != NULL) {
        pushLeft(iter, node->right);
    }

    *result = node->value;
    return true;
}

void iteratorFree(Iterator* iter)
{
    if (iter == NULL) {
        return;
    }
    free(iter->stack);
    free(iter);
}
bool bstMax(const BST* tree, int* result)
{
    if (tree == NULL || tree->root == NULL) {
        return false;
    }
    Node* current = tree->root;
    while (current->right != NULL) {
        current = current->right;
    }
    *result = current->value;
    return true;
}

bool bstMin(const BST* tree, int* result)
{
    if (tree == NULL || tree->root == NULL) {
        return false;
    }
    Node* current = tree->root;
    while (current->left != NULL) {
        current = current->left;
    }
    *result = current->value;
    return true;
}

int bstSize(const BST* tree)
{
    if (tree == NULL) {
        return 0;
    }
    return tree->size;
}

static int nodeHeight(const Node* node)
{
    if (node == NULL) {
        return 0;
    }

    int leftHeight = nodeHeight(node->left);
    int rightHeight = nodeHeight(node->right);

    return (1 + ((leftHeight >= rightHeight) ? leftHeight : rightHeight));
}

int bstHeight(const BST* tree)
{
    if (tree == NULL) {
        return 0;
    }
    return nodeHeight(tree->root);
}
static void isValid(const Node* node, int min, int max, bool* fl)
{
    if (!(*fl) || node == NULL) {
        return;
    }

    if (node->value <= min || node->value >= max) {
        *fl = false;
        return;
    }

    isValid(node->left, min, node->value, fl);

    isValid(node->right, node->value, max, fl);
}

bool bstIsValid(const BST* tree)
{
    if (tree == NULL) {
        return false;
    }
    bool fl = true;
    isValid(tree->root, INT_MIN, INT_MAX, &fl);
    return fl;
}

bool bstKthMin(const BST* tree, int k, int* result)
{
    if (k <= 0 || tree == NULL || k > bstSize(tree)) {
        return false;
    }
    Iterator* iter = iteratorInit(tree);
    int value;
    bool ok;
    for (int i = 0; i < k; i++) {
        ok = iteratorNext(iter, &value);
        if (!ok) {
            iteratorFree(iter);
            return false;
        }
    }
    *result = value;
    iteratorFree(iter);
    return true;
}
