#include <stdio.h>

typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct {
    Node* root;
    int size;
} BST;

/*тут будут тулы*/

int max(int first, int second)
{
    return (first > second) ? first : second;
}
//

// реализация высоты дерева из двух функций
int heightNode(Node* node)
{
    if (node == NULL) {
        return 0;
    }

    Node* leftHieght = maxSizeNode(node->left);
    Node* rightHieght = maxSizeNode(node->right);
    return (1 + max(leftHieght, rightHieght));
}

int bstHeight(BST* tree)
{
    if (tree == NULL || tree->root == NULL) {
        return 0;
    }
    return heightNode(tree);
}
//

// количество узлов.
int bstSize(BST* tree)
{
    if (tree == NULL) {
        return 0;
    }
    return tree->size;
}

/*реализация максимума и минимума в дереве. в случае с пустым деревом будет возвращаться ноль*/
int bstMin(BST* tree)
{
    if (tree == NULL || tree->root == NULL) {
        return 0;
    }
    Node* current = tree->root;
    while (current->left != NULL) {
        current = current->left;
    }
    return current->value;
}

int bstMax(BST* tree)
{
    if (tree == NULL || tree->root == NULL) {
        return 0;
    }
    Node* current = tree->root;
    while (current->right != NULL) {
        current = current->right;
    }
    return current->value;
}
