#include "bst.h"
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

Iterator* iteratorInit(BST* tree)
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

bool iteratorHasNext(Iterator* iter)
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

    Node* node = iter->stack[--iter->top];
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