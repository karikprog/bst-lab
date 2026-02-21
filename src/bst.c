#include "bst.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

BST* InitTree()
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

void bstFree(BST* tree)
{
    BST** treeRef = &tree;
    if (treeRef == NULL || *treeRef == NULL) {
        return;
    }

    cleanTree(tree->root);
    free(tree);
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
    if (search(tree->root, value) != NULL) {
        return true;
    }
    return false;
}

int bstInsert(BST* tree, int value)
{
    if (bstContains(tree, value) == true) {
        return 0;
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
    tree->size++;
    while (1) {
        if (node->value > value) {
            if (node->left == NULL) {
                node->left = temp;
                return 0;
            } else {
                node = node->left;
            }

        } else {
            if (node->right == NULL) {
                node->right = temp;
                return 0;
            } else {
                node = node->right;
            }
        }
    }
}