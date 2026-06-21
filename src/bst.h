#pragma once
#include <stdbool.h>

typedef struct BST BST;
typedef struct Iterator Iterator;

// Allocates a BST container on the heap, initializing root to NULL and size to 0.
BST* initTree();

// Recursively deallocates all nodes, frees the BST structure, and nullifies the caller's pointer.
void bstFree(BST** treeRef);

// Wrapper for search that returns true if the value is found, false otherwise.
bool bstContains(const BST* tree, int value);

// Iteratively inserts a value; returns -1 on allocation failure or 0 on success (including duplicates).
int bstInsert(BST* tree, int value);

// Initializes a stack-based iterator for in-order traversal with capacity equal to tree size.
Iterator* iteratorInit(const BST* tree);

// Returns true if the iterator's internal stack is not empty.
bool iteratorHasNext(const Iterator* iter);

// Advances traversal, updates the stack with the right child's left spine, and returns the next value.
bool iteratorNext(Iterator* iter, int* result);

// Frees the iterator's internal stack and the iterator structure itself.
void iteratorFree(Iterator* iter);

// Traverses to the rightmost node to retrieve the maximum value in the tree.
bool bstMax(const BST* tree, int* result);

// Traverses to the leftmost node to retrieve the minimum value in the tree.
bool bstMin(const BST* tree, int* result);

// Returns the cached size of the tree (number of nodes).
int bstSize(const BST* tree);

// Recursively calculates the maximum depth (height) of the tree.
int bstHeight(const BST* tree);

// Validates the BST property by ensuring every node stays within its logical [min, max] bounds.
bool bstIsValid(const BST* tree);

// Iteratively removes a node and replaces it with a predecessor or successor to maintain structure.
void bstDelete(BST* tree, int value);

// Uses an in-order iterator to find the k-th smallest element (1-indexed).
bool bstKthMin(const BST* tree, int k, int* result);

// Returns a heap-allocated array of values collected via in-order (LNR) traversal.
int* bstInorder(const BST* tree);

// Returns a heap-allocated array of values collected via pre-order (NLR) traversal.
int* bstPreorder(const BST* tree);

// Returns a heap-allocated array of values collected via post-order (LRN) traversal.
int* bstPostorder(const BST* tree);

// Creates a new height-balanced BST by sorting combined values from two trees and picking medians.
BST* bstMerge(const BST* tree1, const BST* tree2);