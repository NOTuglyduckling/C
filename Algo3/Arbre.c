#include <stdio.h>
#include <stdlib.h>

BSTree* nca(const BSTree* t, int k1, int k2);
int distance(const BSTree* t, int k1, int k2);
int findDepth(BSTree* root, int key, int depth);

// BSTree structure definition
typedef struct s_bstree {
    int key;
    struct s_bstree *left;
    struct s_bstree *right;
} BSTnode, *BSTree;

// Function to find distance between two keys
int distance(const BSTree* t, int k1, int k2) {
    if (t == NULL) return -1;
    
    // First find the nearest common ancestor
    BSTree* nca = nca(t, k1, k2);
    if (nca == NULL) return -1;
    
    // Calculate depths of both keys
    int depth1 = findDepth(nca, k1, 0);
    int depth2 = findDepth(nca, k2, 0);
    
    // Total distance is sum of depths from NCA
    return depth1 + depth2;
}

// Helper function to find depth of a key from given root
int findDepth(BSTree* root, int key, int depth) {
    if (root == NULL) return -1;
    if (root->key == key) return depth;
    
    int leftDepth = findDepth(root->left, key, depth + 1);
    if (leftDepth != -1) return leftDepth;
    
    return findDepth(root->right, key, depth + 1);
}

// Function to find nearest common ancestor
BSTree* nca(const BSTree* t, int k1, int k2) {
    if (t == NULL) return NULL;
    
    // If both k1 and k2 are smaller than root, go left
    if (k1 < t->key && k2 < t->key)
        return nca(t->left, k1, k2);
    
    // If both k1 and k2 are greater than root, go right
    if (k1 > t->key && k2 > t->key)
        return nca(t->right, k1, k2);
    
    // If one key is smaller and other is greater, current node is NCA
    return t;
}