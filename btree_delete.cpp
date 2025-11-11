#include "btree.h"

/*
NOTE: Please follow logic from CLRSv4 directly. Additionally, in cases 3a and 3b please check for an immediate right sibling first.
*/


// 1 Lukas
// delete the key k from the btree

// Precondition:
// Postcondition:

void BTree::remove(int k) {

    if (not root){
        return;
    }

    remove(root, k ,true) // removing the node that has key is k

    // 1. If the number of keys (n) on the root node is 0 (root -> n == 0) 
    // 2. if the root is not a leaf node (!root -> leaf)
    // if statement is true, so execute it.

    if (root -> n == 0 && !root -> leaf) { // ke == 0 and root children count == 1 which is c[0]

        Node *old_Root = root; // temporary save node 

        root = root -> c[0] // root = root -> c[0]

        delete old_Root; // prevent memory leak

    }
}

// delete the key k from the btree rooted at x

// Precondition:
// Postcondition:

void BTree::remove(Node *x, int k, bool x_root)
{

}

// return the index i of the first key in the btree node x where k <= x.keys[i]
// if i = x.n then no such key exists

// Precondition:
// Postcondition:

int BTree::find_k(Node *x, int k) {

    int i = 0;

    while (i < x-> n && k > x -> keys[i]) {
        i++;
    }

    return i;

}





// 2 Sunho 
// remove the key at index i from a btree leaf node x
void BTree::remove_leaf_key(Node *x, int i)
{
}

// remove the key at index i and child at index j from a btree internal node x
void BTree::remove_internal_key(Node *x, int i, int j)
{
}

// return the max key in the btree rooted at node x
int BTree::max_key(Node *x)
{
}

// return the min key in the btree rooted at node x
int BTree::min_key(Node *x)
{
}

// 3 Flynn
// merge key k and all keys and children from y into y's LEFT sibling x
void BTree::merge_left(Node *x, Node *y, int k)
{
}

// merge key k and all keys and children from y into y's RIGHT sibling x
void BTree::merge_right(Node *x, Node *y, int k)
{
}

// Give y an extra key by moving a key from its parent x down into y
// Move a key from y's LEFT sibling z up into x
// Move appropriate child pointer from z into y
// Let i be the index of the key dividing y and z in x
void BTree::swap_left(Node *x, Node *y, Node *z, int i)
{
}

// Give y an extra key by moving a key from its parent x down into y
// Move a key from y's RIGHT sibling z up into x
// Move appropriate child pointer from z into y
// Let i be the index of the key dividing y and z in x
void BTree::swap_right(Node *x, Node *y, Node *z, int i)
{
}