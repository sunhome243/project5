#include "btree.h"

/*
NOTE: Please follow logic from CLRSv4 directly. Additionally, in cases 3a and 3b please check for an immediate right sibling first.
*/

// delete the key k from the btree

// Precondition: BTree is not empty
// Postcondition: Key k is removed from the BTree if it exists

void BTree::remove(int k)
{

    if (!root)
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
    else if (root->n == 0 && root->leaf) // if the root is leaf node and has 0 keys
    {
        delete root; // delete root
        root = nullptr;
    }
}

// delete the key k from the btree rooted at x

// Precondition : BTree is not empty
// Postcondition : Key k is removed from the BTree if it exists

void BTree::remove(Node *x, int k, bool x_root)
{
    while (x != nullptr)
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

                // remove predecessor from left_node (recurssively)
                remove(left_node, pred, false);
            }

            // Case 2-b: left node has t-1 keys but right node has at least t keys (left node can not be extracted(min key req) but right node is available)
            else if (right_node->n >= t)
            {
                int succ = min_key(right_node);

                x->keys[i] = succ;
                // remove predecessor from left_node (recurssively)
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
        }

        // Case 3: Key k is not in node x
        
        // Case 1, 2 failed and x is leaf node, then there is no key k in the tree.
        if (x->leaf) 
        {
            return; 
        }

        // now we can ensure that x is an internal node and does not contain key k

        else // x is an internal node and does not contain key k
        {
            Node *next = x->c[i];
            Node *left_sib = (i > 0) ? x->c[i - 1] : nullptr;
            Node *right_sib = (i < x->n) ? x->c[i + 1] : nullptr;

            if (next->n == (t - 1))
            {
                // Check right sibling first
                if (right_sib != nullptr && right_sib->n > t - 1)
                {
                    swap_right(x, next, right_sib, i);
                }
                // Then check left sibling
                else if (left_sib != nullptr && left_sib->n > t - 1)
                {
                    swap_left(x, next, left_sib, i - 1);
                }
                else if (right_sib != nullptr) // merge with right sibling
                {
                    merge_left(next, right_sib, x->keys[i]);
                    remove_internal_key(x, i, i + 1);
                }
                else // merge with left sibling
                {
                    merge_left(left_sib, next, x->keys[i - 1]);
                    remove_internal_key(x, i - 1, i);
                    next = left_sib;
                }
            }
            x = next; // update for the next loop
        }
    }
}

// return the index i of the first key in the btree node x where k <= x.keys[i]
// if i = x.n then no such key exists

// Precondition: x is a valid BTree node
// Postcondition: returns the index of the first key greater than or equal to k

int BTree::find_k(Node *x, int k)
{

    int i = 0;

    while (i < x->n && k > x->keys[i])
    {
        i++;
    }

    return i;
}

// remove the key at index i from a btree leaf node x

// Precondition: x is a leaf node and 0 <= i < x->n
// Postcondition: key at index i is removed from node x

void BTree::remove_leaf_key(Node *x, int i)
{
    for (int j = i; j < x->n - 1; j++)
    {
        x->keys[j] = x->keys[j + 1];
    }
    x->n--;
}

// remove the key at index i and child at index j from a btree internal node x

// Precondition: x is an internal node and 0 <= i < x->n, 0 <= j <= x->n
// Postcondition: key at index i and child at index j are removed from node x

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

// Precondition: x is a valid BTree node
// Postcondition: returns the maximum key in the subtree rooted at x

int BTree::max_key(Node *x)
{
    while (!x->leaf)
    {
        x = x->c[x->n];
    }
    return x->keys[x->n - 1];
}

// return the min key in the btree rooted at node x

// Precondition: x is a valid BTree node
// Postcondition: returns the minimum key in the subtree rooted at x

int BTree::min_key(Node *x)
{
    while (!x->leaf)
    {
        x = x->c[0];
    }
    return x->keys[0];
}

// merge key k and all keys and children from y into y's LEFT sibling x

// Precondition: x and y are siblings, and x has t-1 keys
// Postcondition: all keys and children from y and key k are merged into x, and y is deleted

void BTree::merge_left(Node *x, Node *y, int k)
{
    // Add the separating key k to x
    x->keys[x->n] = k;
    
    // Copy all keys from y to x after k
    for (int i = 0; i < y->n; i++) {
        x->keys[x->n + 1 + i] = y->keys[i];
    }
    
    // Copy all children from y to x (if internal node)
    if (!x->leaf) {
        for (int i = 0; i <= y->n; i++) {
            x->c[x->n + 1 + i] = y->c[i];
        }
    }
    
    // Update x's key count (original keys + separator + y's keys)
    x->n += y->n + 1;
    
    // Delete the now-empty node y
    delete y;
}

// merge key k and all keys and children from y into y's RIGHT sibling x

// Precondition: x and y are siblings, and x has t-1 keys
// Postcondition: all keys and children from y and key k are merged into x, and y is deleted

void BTree::merge_right(Node *x, Node *y, int k)
{
    // Shift x's existing keys right to make room for y's keys and k
    for (int i = x->n - 1; i >= 0; i--) {
        x->keys[i + y->n + 1] = x->keys[i];
    }
    
    // Shift x's existing children right (if internal node)
    if (!x->leaf) {
        for (int i = x->n; i >= 0; i--) {
            x->c[i + y->n + 1] = x->c[i];
        }
    }
    
    // Copy y's keys to the beginning of x
    for (int i = 0; i < y->n; i++) {
        x->keys[i] = y->keys[i];
    }
    
    // Place separator key k after y's keys
    x->keys[y->n] = k;
    
    // Copy y's children to the beginning of x (if internal node)
    if (!x->leaf) {
        for (int i = 0; i <= y->n; i++) {
            x->c[i] = y->c[i];
        }
    }
    
    // Update x's key count
    x->n += y->n + 1;
    
    // Delete the now-empty node y
    delete y;
}

// Give y an extra key by moving a key from its parent x down into y
// Move a key from y's LEFT sibling z up into x
// Move appropriate child pointer from z into y
// Let i be the index of the key dividing y and z in x

// Precondition: y has t-1 keys and z has at least t keys
// Postcondition: y has one more key, z has one less key, and the parent x is updated accordingly.

void BTree::swap_left(Node *x, Node *y, Node *z, int i)
{
    // Shift y's keys right to make room at the beginning
    for (int j = y->n - 1; j >= 0; j--) {
        y->keys[j + 1] = y->keys[j];
    }
    
    // Shift y's children right (if internal node)
    if (!y->leaf) {
        for (int j = y->n; j >= 0; j--) {
            y->c[j + 1] = y->c[j];
        }
    }
    
    // Move parent's separating key down to y
    y->keys[0] = x->keys[i];
    
    // Move z's rightmost child to y's leftmost position (if internal)
    if (!y->leaf) {
        y->c[0] = z->c[z->n];
    }
    
    // Move z's rightmost key up to parent
    x->keys[i] = z->keys[z->n - 1];
    
    // Update key counts
    y->n++;
    z->n--;
}

// Give y an extra key by moving a key from its parent x down into y
// Move a key from y's RIGHT sibling z up into x
// Move appropriate child pointer from z into y
// Let i be the index of the key dividing y and z in x

// Precondition: y has t-1 keys and z has at least t keys
// Postcondition: y has one more key, z has one less key, and the parent x is updated accordingly.

void BTree::swap_right(Node *x, Node *y, Node *z, int i)
{
    // Move parent's separating key down to end of y
    y->keys[y->n] = x->keys[i];
    
    // Move z's leftmost child to y's rightmost position (if internal)
    if (!y->leaf) {
        y->c[y->n + 1] = z->c[0];
    }
    
    // Move z's leftmost key up to parent
    x->keys[i] = z->keys[0];
    
    // Shift z's remaining keys left
    for (int j = 0; j < z->n - 1; j++) {
        z->keys[j] = z->keys[j + 1];
    }
    
    // Shift z's remaining children left (if internal node)
    if (!z->leaf) {
        for (int j = 0; j < z->n; j++) {
            z->c[j] = z->c[j + 1];
        }
    }
    
    // Update key counts
    y->n++;
    z->n--;
}