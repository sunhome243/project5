#include "btree.h"

/*
NOTE: Please follow logic from CLRSv4 directly. Additionally, in cases 3a and 3b please check for an immediate right sibling first.
*/

// 1 Lukas
// delete the key k from the btree

// Precondition:
// Postcondition:

void BTree::remove(int k)
{

    if (not root)
    {
        return;
    }

    remove(root, k, true);

    // removing the node that has key is k
    // 1. If the number of keys (n) on the root node is 0 (root -> n == 0)
    // 2. if the root is not a leaf node (!root -> leaf)
    // if statement is true, so execute it.

    if (root->n == 0 && !root->leaf)
    {
        Node *old_Root = root; // temporary save node
        root = root->c[0];     // root = root -> c[0]
        delete old_Root;       // prevent memory leak
    }
}

// delete the key k from the btree rooted at x

// Precondition:
// Postcondition:

void BTree::remove(Node *x, int k, bool x_root)
{
    int i = find_k(x, k);

    // Case 1 : Key k in node x, and x is leaf node.

    if (i < x->n && x->keys[i] == k && x->leaf)
    {
        remove_leaf_key(x, i);
        return;
    }

    // Case 2:  Key k in node x, and x is inside node (not leaf).

    if (i < x->n && x->keys[i] == k && !x->leaf)
    {
        // left and right node of key k that should delete. (left child= precede k, right child= follows k)
        Node *left_node = x->c[i];      // Left-side
        Node *right_node = x->c[i + 1]; // Right-side

        // Case 2-a: left child node left_node has at least t nodes
        if (left_node->n >= t)
        {
            // find k's predecessor from left_node
            int pred = max_key(left_node);

            // replace x.keys[i] into predecessor
            x->keys[i] = pred;

            // k가 아닌 선행 키 pred'를 left_node의 서브트리에서 재귀적으로 삭제
            remove(left_node, pred, false);
        }

        // Case 2-b: left node has t-1 keys and right node has at least t keys (left node can not be extracted(min key req) but right node is available)
        else if (right_node->n >= t)
        {
            int succ = min_key(right_node);

            x->keys[i] = succ;

            remove(right_node, succ, false);
        }

        // Case 2-c: both left and right node is not available since both has t-1 keys 
        else
        {

            merge_left(left_node, right_node, k);

            remove_internal_key(x, i, i + 1);

            remove(left_node, k, false);
        }
        return; // Finishing up the Case 2

        // I have to continue and finish up Case 3.
    }
}

// return the index i of the first key in the btree node x where k <= x.keys[i]
// if i = x.n then no such key exists

// Precondition:
// Postcondition:

int BTree::find_k(Node *x, int k)
{

    int i = 0;

    while (i < x->n && k > x->keys[i])
    {
        i++;
    }

    return i;
}

// 2 Sunho
// remove the key at index i from a btree leaf node x
void BTree::remove_leaf_key(Node *x, int i)
{
    for (int j = i; j < x->n - 1; j++)
    {
        x->keys[j] = x->keys[j + 1];
    }
    x->n--;
}

// remove the key at index i and child at index j from a btree internal node x
void BTree::remove_internal_key(Node *x, int i, int j)
{
    for (int k = i; k < x->n - 1; k++)
    {
        x->keys[k] = x->keys[k + 1];
    }
    for (int k = j; k < x->n; k++)
    {
        x->c[k] = x->c[k + 1];
    }
    x->c[x->n] = nullptr;
    x->n--;
}

// return the max key in the btree rooted at node x
int BTree::max_key(Node *x)
{
    while (!x->leaf)
    {
        x = x->c[x->n - 1];
    }
    return x->keys[x->n - 1];
}

// return the min key in the btree rooted at node x
int BTree::min_key(Node *x)
{
    while (!x->leaf)
    {
        x = x->c[0];
    }
    return x->keys[0];
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