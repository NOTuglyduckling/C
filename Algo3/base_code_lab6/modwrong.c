#include "bstree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"

/*------------------------  BSTreeType  -----------------------------*/

typedef enum {red,black} NodeColor;

struct _bstree {
    BinarySearchTree* parent;
    BinarySearchTree* left;
    BinarySearchTree* right;
    int key;
    NodeColor color;
};

/*-----------------------------------------Exercice 4 TP5-------------------------------------------*/

typedef const BinarySearchTree* (*AccessFunction)(const BinarySearchTree* node);

static const BinarySearchTree* access_left(const BinarySearchTree* node) {
    return node ? node->left : NULL;
}

static const BinarySearchTree* access_right(const BinarySearchTree* node) {
    return node ? node->right : NULL;
}

typedef struct {
    AccessFunction left;
    AccessFunction right;
} ChildAccessors;

/*-------------------------- Affichage Rouge Noir -------------------------*/

void bstree_node_to_dot(const BinarySearchTree *t, void *stream) {
    FILE *file = (FILE *) stream;
    const char *color = (t->color == black) ? "grey" : "red";

    printf("%d ", bstree_key(t));
    fprintf(file, "\tn%d [style=filled, fillcolor=%s, fontcolor=white, label=\"{%d|{<left>|<right>}}\"];\n",
            bstree_key(t), color, bstree_key(t));

    if (bstree_left(t)) {
        fprintf(file, "\tn%d:left:c -> n%d:n [headclip=false, tailclip=false]\n",
                bstree_key(t), bstree_key(bstree_left(t)));
    } else {
        fprintf(file, "\tlnil%d [style=filled, fillcolor=grey, label=\"NIL\"];\n", bstree_key(t));
        fprintf(file, "\tn%d:left:c -> lnil%d:n [headclip=false, tailclip=false]\n",
                bstree_key(t), bstree_key(t));
    }
    if (bstree_right(t)) {
        fprintf(file, "\tn%d:right:c -> n%d:n [headclip=false, tailclip=false]\n",
                bstree_key(t), bstree_key(bstree_right(t)));
    } else {
        fprintf(file, "\trnil%d [style=filled, fillcolor=grey, label=\"NIL\"];\n", bstree_key(t));
        fprintf(file, "\tn%d:right:c -> rnil%d:n [headclip=false, tailclip=false]\n",
                bstree_key(t), bstree_key(t));
    }
}

/*------------------------- Rouge Noir ------------------------------*/

void rightrotate(BinarySearchTree* y) ;
void leftrotate(BinarySearchTree* x);

void testrotateleft(BinarySearchTree *t){
    leftrotate(t);
}

void testrotateright(BinarySearchTree *t){
    rightrotate(t);
}

BinarySearchTree* grandparent(BinarySearchTree* n);
BinarySearchTree* uncle(BinarySearchTree* n);

BinarySearchTree* fixredblack_insert(ptrBinarySearchTree x);
BinarySearchTree* fixredblack_insert_case1(ptrBinarySearchTree x, ChildAccessors access);
BinarySearchTree* fixredblack_insert_case2(ptrBinarySearchTree x, ChildAccessors access);

BinarySearchTree* fixredblack_remove(BinarySearchTree* p, BinarySearchTree* x);
BinarySearchTree* fixredblack_remove_case1(BinarySearchTree* p, ChildAccessors access);
BinarySearchTree* fixredblack_remove_case2(BinarySearchTree* p, ChildAccessors access);

/*------------------------  BaseBSTree  -----------------------------*/

BinarySearchTree* bstree_create(void) {
    return NULL;
}

/* This constructor is private so that we can maintain the oredring invariant on
 * nodes. The only way to add nodes to the tree is with the bstree_add function
 * that ensures the invariant.
 */
BinarySearchTree* bstree_cons(BinarySearchTree* left, BinarySearchTree* right, int key) {
    BinarySearchTree* t = malloc(sizeof(struct _bstree));
    t->parent = NULL;
    t->left = left;
    t->right = right;
    if (t->left != NULL)
        t->left->parent = t;
    if (t->right != NULL)
        t->right->parent = t;
    t->key = key;
    t->color = red;
    return t;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void free_node(const BinarySearchTree* t, void* n) {
    (void)n;
    free((BinarySearchTree*)t);
}

void bstree_delete(ptrBinarySearchTree* t) {
    bstree_depth_postfix(*t, free_node, NULL);
    *t=NULL;
}

bool bstree_empty(const BinarySearchTree* t) {
    return t == NULL;
}

int bstree_key(const BinarySearchTree* t) {
    assert(!bstree_empty(t));
    return t->key;
}

BinarySearchTree* bstree_left(const BinarySearchTree* t) {
    assert(!bstree_empty(t));
    return t->left;
}

BinarySearchTree* bstree_right(const BinarySearchTree* t) {
    assert(!bstree_empty(t));
    return t->right;
}

BinarySearchTree* bstree_parent(const BinarySearchTree* t) {
    assert(!bstree_empty(t));
    return t->parent;
}

/*------------------------  BSTreeDictionary  -----------------------------*/

/* Obligation de passer l'arbre par référence pour pouvoir le modifier */
void bstree_add(ptrBinarySearchTree* t, int v) {
    if (!*t) {
        *t = bstree_cons(NULL, NULL, v);
        (*t)->color = black; // Root is always black
        return;
    }

    BinarySearchTree* current = *t;
    BinarySearchTree* parent = NULL;

    while (current) {
        parent = current;
        if (v < bstree_key(current)) {
            current = current->left;
        } else if (v > bstree_key(current)) {
            current = current->right;
        } else {
            // Duplicate keys are not allowed
            return;
        }
    }

    BinarySearchTree* newNode = bstree_cons(NULL, NULL, v);
    newNode->parent = parent;

    if (v < bstree_key(parent)) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }
    BinarySearchTree* stop =fixredblack_insert(newNode);
    
    // Mise à jour de la racine si nécessaire
    if (stop && !stop->parent)
        *t = stop;
}

const BinarySearchTree* bstree_search(const BinarySearchTree* t, int v) {
	assert(!bstree_empty(t));
    const BinarySearchTree* cur = t; 
    while(cur){
        if (v==bstree_key(cur))
            return cur;
        if (v>bstree_key(cur))
            cur =cur->right;
        else
            cur = cur->left;
    }
    return NULL;
}

/*const BinarySearchTree* bstree_successor(const BinarySearchTree* x) {
    assert(!bstree_empty(x));
    const BinarySearchTree* cur = x->right; 
    if (cur){
        while (cur->left)
            cur = cur->left;
    } else {
        cur=x->parent;
        while(cur && x==cur->right)
            cur = cur->parent;
    }
    return cur;
}

const BinarySearchTree* bstree_predecessor(const BinarySearchTree* x) {
    assert(!bstree_empty(x));
    const BinarySearchTree* cur = x->left; 
    if (cur){
        while (cur->right)
            cur = cur->right;
    } else {
        cur=x->parent;
        while(cur && x==cur->left){
            x=cur;
            cur = cur->parent;
        }   
    }
    return cur;
}*/

static const BinarySearchTree* find_next(const BinarySearchTree* x, ChildAccessors access) {
    assert(x != NULL);

    if (access.right(x)) {
        const BinarySearchTree* cur = access.right(x);
        while (access.left(cur)) {
            cur = access.left(cur);
        }
        return cur;
    } else {
        const BinarySearchTree* cur = x->parent;
        while (cur && x == access.right(cur)) {
            x = cur;
            cur = cur->parent;
        }
        return cur;
    }
}

const BinarySearchTree* bstree_successor(const BinarySearchTree* x) {
    assert(x != NULL);
    ChildAccessors successor_access = { access_left, access_right };
    return find_next(x, successor_access);
}

const BinarySearchTree* bstree_predecessor(const BinarySearchTree* x) {
    assert(x != NULL);
    ChildAccessors predecessor_access = { access_right, access_left };
    return find_next(x, predecessor_access);
}

void bstree_swap_nodes(ptrBinarySearchTree* tree, ptrBinarySearchTree from, ptrBinarySearchTree to) {
    assert(!bstree_empty(*tree) && !bstree_empty(from) && !bstree_empty(to));
    if (from == to) return;

    BinarySearchTree* parent_from = from->parent;
    BinarySearchTree* parent_to = to->parent;
    BinarySearchTree* left_from = from->left;
    BinarySearchTree* right_from = from->right;
    BinarySearchTree* left_to = to->left;
    BinarySearchTree* right_to = to->right;

    if (parent_from) {
        if (parent_from->left == from) parent_from->left = to;
        else parent_from->right = to;
    }
    if (parent_to) {
        if (parent_to->left == to) parent_to->left = from;
        else parent_to->right = from;
    }

    // Swapping children
    from->left = left_to;
    from->right = right_to;
    to->left = left_from;
    to->right = right_from;

    if (left_from) left_from->parent = to;
    if (right_from) right_from->parent = to;
    if (left_to) left_to->parent = from;
    if (right_to) right_to->parent = from;

    // Update parent links for swapped nodes
    from->parent = parent_to;
    to->parent = parent_from;

    // Update root if necessary
    if (*tree == from) *tree = to;
    else if (*tree == to) *tree = from;
}


// t -> the tree to remove from, cur -> the node to remove
void bstree_remove_node(ptrBinarySearchTree* t, ptrBinarySearchTree current) {
    assert(!bstree_empty(*t) && !bstree_empty(current));

    ptrBinarySearchTree substitute = NULL;

    if (current->left == NULL || current->right == NULL) {
        // Node has at most one child or is a leaf
        substitute = (current->left != NULL) ? current->left : current->right;
    } else {
        // Node has two children: Find in-order successor
        substitute = (ptrBinarySearchTree)bstree_successor(current);
        current->key = substitute->key; // Copy key from successor
        bstree_remove_node(t, substitute); // Recursively remove successor
        return; // Successor is removed, no further action needed here
    }

    // Update parent pointers
    if (current->parent) {
        if (current == current->parent->left) {
            current->parent->left = substitute;
        } else {
            current->parent->right = substitute;
        }
    } else {
        // Current node is the root
        *t = substitute;
    }

    if (substitute) {
        substitute->parent = current->parent;
    }

    if (current->color == black) {
        if (!substitute || substitute->color == black) {
            BinarySearchTree* fixedRoot = fixredblack_remove(current->parent, substitute);
            if (!fixedRoot->parent) {
                *t = fixedRoot;
            }
        } else {
            substitute->color = black;
        }
    }
    free(current); // Free the removed node
}


void bstree_remove(ptrBinarySearchTree* t, int v) {
    ptrBinarySearchTree current = *t;

    // Search for the node to remove
    while (current) {
        if (v == current->key) {
            break;
        }
        current = (v < current->key) ? current->left : current->right;
    }

    // If the node is found, proceed with removal
    if (current) {
        bstree_remove_node(t, current);
    }
}



/*------------------------------EXERCICE 6-2------------------------------*/

void leftrotate(BinarySearchTree* x) {
    BinarySearchTree* y = x->right;
    assert(y);  // y must exist for rotation

    x->right = y->left;
    if (y->left) {
        y->left->parent = x;
    }

    y->parent = x->parent;
    if (!x->parent) {
        // x is root
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

void rightrotate(BinarySearchTree* y) {
    BinarySearchTree* x = y->left;
    assert(x);  // x must exist for rotation

    y->left = x->right;
    if (x->right) {
        x->right->parent = y;
    }

    x->parent = y->parent;
    if (!y->parent) {
        // y is root
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }

    x->right = y;
    y->parent = x;
}


/*-----------------------------------------EXERCICE 6-3------------------------------------------*/

BinarySearchTree* grandparent(BinarySearchTree* n) {
    assert(!bstree_empty(n)&&!bstree_empty(n->parent)&&!bstree_empty(n->parent->parent));
    return n->parent->parent;
}


BinarySearchTree* uncle(BinarySearchTree* n) {
    assert(!bstree_empty(n)&&!bstree_empty(n->parent)&&!bstree_empty(n->parent->parent));
    BinarySearchTree* pp = grandparent(n);
    if (n->parent == pp->right)
        return pp->left;
    else
        return pp->right;
}


// Fonction principale de correction après insertion
BinarySearchTree* fixredblack_insert(ptrBinarySearchTree x) {
    assert(!bstree_empty(x));

    if (!x->parent) {
        x->color = black;
        return x;
    }

    ChildAccessors access = (x == x->parent->left)
        ? (ChildAccessors){access_left, access_right}
        : (ChildAccessors){access_right, access_left};

    // Passer aux cas de correction
    return fixredblack_insert_case1(x,access);
}

BinarySearchTree* fixredblack_insert_case1(ptrBinarySearchTree x, ChildAccessors access) {
    assert(!bstree_empty(x));
    if (!x->parent || !x->parent->parent) 
        return x;

    BinarySearchTree* p = x->parent;
    BinarySearchTree* pp = grandparent(x);
    BinarySearchTree* u = uncle(x);

    if (u && u->color == red) {
        // Case 1: Uncle is red -> Recolor and recurse
        p->color = black;
        u->color = black;
        pp->color = red;
        return fixredblack_insert(pp);
    }

    return fixredblack_insert_case2(x,access);
}

BinarySearchTree* fixredblack_insert_case2(ptrBinarySearchTree x, ChildAccessors access) {
    BinarySearchTree* p = x->parent;
    BinarySearchTree* pp = grandparent(x);

    // Handle rotations
    if (x == access.right(p)) {
        access.left == access_left ? leftrotate(p) : rightrotate(p);
        x = p;  // Adjust x for final rotation
    }

    // Recompute parent and grandparent
    p = x->parent;
    pp = grandparent(x);

    // Perform the second rotation
    access.left == access_left ? rightrotate(pp) : leftrotate(pp);

    // Recolor
    p->color = black;
    pp->color = red;

    return p;
}

/*--------------------------------------------FIX REMOVE----------------------------------------------*/

BinarySearchTree* fixredblack_remove(BinarySearchTree* p, BinarySearchTree* x) {
    if (!p) {
        // If p is NULL, x is the root. Ensure it's black.
        if (x) x->color = black;
        return x;
    }

    // Determine sibling and create accessors for symmetry
    ChildAccessors access = (x == p->left)
        ? (ChildAccessors){access_right, access_left}
        : (ChildAccessors){access_left, access_right};

    BinarySearchTree* sibling = (BinarySearchTree*)access.right(p);

    // Case 1: Sibling is black
    if (!sibling || sibling->color == black) {
        return fixredblack_remove_case1(p,access);
    }
    // Case 2: Sibling is red
    else {
        return fixredblack_remove_case2(p, access);
    }
}

BinarySearchTree* fixredblack_remove_case1(BinarySearchTree* p, ChildAccessors access) {
    BinarySearchTree* sibling = (BinarySearchTree*)access.right(p);

    // Check sibling's children
    BinarySearchTree* closeChild = (BinarySearchTree*)access.left(sibling);
    BinarySearchTree* farChild = (BinarySearchTree*)access.right(sibling);

    // Case 1.1: Sibling and its children are black
    if ((!closeChild || closeChild->color == black) && (!farChild || farChild->color == black)) {
        sibling->color = red; // Recolor sibling to red
        if (p->color == red) {
            p->color = black; // Fix by recoloring parent
        } else {
            // Propagate double-black to parent
            return fixredblack_remove(p->parent, p);
        }
    }
    // Case 1.2: Far child of sibling is red
    else if (farChild && farChild->color == red) {
        sibling->color = p->color; // Inherit parent's color
        p->color = black;
        farChild->color = black; // Recolor far child

        // Perform rotation
        if (access.left == access_left) {
            leftrotate(p);
        } else {
            rightrotate(p);
        }
    }
    // Case 1.3: Close child of sibling is red
    else if (closeChild && closeChild->color == red) {
        closeChild->color = black;
        sibling->color = red;

        // Perform rotation to prepare for far child case
        if (access.left == access_left) {
            rightrotate(sibling);
        } else {
            leftrotate(sibling);
        }

        // Recompute sibling and process again
        sibling = (BinarySearchTree*)access.right(p);
        sibling->color = p->color;
        p->color = black;
        ((BinarySearchTree*)access.right(sibling))->color = black;

        // Final rotation
        if (access.left == access_left) {
            leftrotate(p);
        } else {
            rightrotate(p);
        }
    }
    return p;
}

BinarySearchTree* fixredblack_remove_case2(BinarySearchTree* p, ChildAccessors access) {
    BinarySearchTree* sibling = (BinarySearchTree*)access.right(p);

    // Recolor sibling and parent
    sibling->color = black;
    p->color = red;

    // Perform rotation
    if (access.left == access_left) {
        leftrotate(p);
    } else {
        rightrotate(p);
    }

    // Recompute sibling and recurse
    sibling = (BinarySearchTree*)access.right(p);
    return fixredblack_remove_case1(p, access);
}


/*------------------------  BSTreeVisitors  -----------------------------*/

void bstree_depth_prefix(const BinarySearchTree* t, OperateFunctor f, void* environment) {
    if (bstree_empty(t)) return;
    f(t, environment);
    bstree_depth_prefix(t->left, f, environment);
    bstree_depth_prefix(t->right, f, environment);
}

void bstree_depth_infix(const BinarySearchTree* t, OperateFunctor f, void* environment) {
    if (bstree_empty(t)) return;
    bstree_depth_infix(t->left, f, environment);
    f(t, environment);
    bstree_depth_infix(t->right, f, environment);
}

void bstree_depth_postfix(const BinarySearchTree* t, OperateFunctor f, void* environment) {
    if (bstree_empty(t)) return;
    bstree_depth_postfix(t->left, f, environment);
    bstree_depth_postfix(t->right, f, environment);
    f(t, environment);
}

void bstree_iterative_breadth(const BinarySearchTree* t, OperateFunctor f, void* environment) {
    assert(!bstree_empty(t));
    ptrQueue queue = create_queue();
    queue = queue_push(queue, t);
    while (!queue_empty(queue)) {
        const BinarySearchTree* current = (const BinarySearchTree*)queue_top(queue);
        f(current, environment);
        queue = queue_pop(queue);
        if (current->left) 
            queue = queue_push(queue, current->left);
        if (current->right) 
            queue = queue_push(queue, current->right);
    }
    delete_queue(&queue);
}

void bstree_iterative_depth_infix(const BinarySearchTree* t, OperateFunctor f, void* userData) {
    const BinarySearchTree* current = t;
    const BinarySearchTree* prev = NULL;

    while (current) {
        if (!prev || prev == current->parent) {
            // Descendre a fauche si possible
            if (current->left) {
                prev = current;
                current = current->left;
            } else {
                // Visiter le noeud courant
                f(current, userData);
                prev = current;
                current = current->right ? current->right : current->parent;
            }
        } else if (prev == current->left) {
            // revenir du fils gauche
            f(current, userData);
            prev = current;
            current = current->right ? current->right : current->parent;
        } else {
            // revenir du fils droit
            prev = current;
            current = current->parent;
        }
    }
}

/*------------------------  BSTreeIterator  -----------------------------*/

struct _BSTreeIterator {
    /* the collection the iterator is attached to */
    const BinarySearchTree* collection;
    /* the first element according to the iterator direction */
    const BinarySearchTree* (*begin)(const BinarySearchTree* );
    /* the cur element pointed by the iterator */
    const BinarySearchTree* cur;
    /* function that goes to the next element according to the iterator direction */
    const BinarySearchTree* (*next)(const BinarySearchTree* );
};

/* minimum element of the collection */
const BinarySearchTree* goto_min(const BinarySearchTree* e) {
    if (!e) return NULL; // Cas de base : arbre vide
    while (e->left) {    // Descendre à gauche jusqu'à trouver le noeud le plus petit
        e = e->left;
    }
    return e;
}

/* maximum element of the collection */
const BinarySearchTree* goto_max(const BinarySearchTree* e) {
    if (!e) return NULL; // Cas de base : arbre vide
    while (e->right) {   // Descendre à droite jusqu'à trouver le noeud le plus grand
        e = e->right;
    }
    return e;
}

/* constructor */
BSTreeIterator* bstree_iterator_create(const BinarySearchTree* collection, IteratorDirection direction) {
    if (!collection) return NULL;

    // Allouer de la mémoire pour l'itérateur
    BSTreeIterator* iterator = (BSTreeIterator*)malloc(sizeof(BSTreeIterator));
    if (!iterator) return NULL;

    // Initialiser les champs de l'itérateur
    iterator->collection = collection;
    iterator->begin = (direction == forward) ? goto_min : goto_max;
    iterator->next = (direction == forward) ? bstree_successor : bstree_predecessor;

    // Positionner au début
    iterator->cur = iterator->begin(collection);

    return iterator;
}

/* destructor */
void bstree_iterator_delete(ptrBSTreeIterator* iterator) {
    if (iterator && *iterator) {
        free(*iterator);
        *iterator = NULL;
    }
}

BSTreeIterator* bstree_iterator_begin(BSTreeIterator* iterator) {
    if (!iterator) return NULL;
    iterator->cur = iterator->begin(iterator->collection);
    return iterator;
}

bool bstree_iterator_end(const BSTreeIterator* iterator) {
    return !iterator || !iterator->cur;
}

BSTreeIterator* bstree_iterator_next(BSTreeIterator* iterator) {
    if (!iterator || !iterator->cur) return NULL;
    iterator->cur = iterator->next(iterator->cur);
    return iterator;
}

const BinarySearchTree* bstree_iterator_value(const BSTreeIterator* iterator) {
    return iterator ? iterator->cur : NULL;
}