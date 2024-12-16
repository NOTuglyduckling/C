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

/*-----------------------------------------Exercice 5-4-------------------------------------------*/

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
    const char *color ;
    if (t->color == black)
        color ="grey";
    else
        color="red";

    printf("%d ", bstree_key(t));
    fprintf(file, "\tn%d [style=filled, fillcolor=%s, fontcolor=white, label=\"{%d|{<left>|<right>}}\"];\n",
            bstree_key(t), color, bstree_key(t));

    if (bstree_left(t)) {
        fprintf(file, "\tn%d:left:c -> n%d:n [headclip=false, tailclip=false]\n",
                bstree_key(t), bstree_key(bstree_left(t)));
    } else {
        fprintf(file, "\tlnil%d [style=filled, fillcolor=grey, label=\"NIL\"];\n", bstree_key(t));
        fprintf(file, "\tn%d:left:c -> lnil%d:n [fillcolor=grey, headclip=false, tailclip=false]\n",
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
BinarySearchTree* fixredblack_insert_case1(ptrBinarySearchTree x, ChildAccessors accessors);
BinarySearchTree* fixredblack_insert_case2(ptrBinarySearchTree x, ChildAccessors accessors);

BinarySearchTree* fixredblack_remove(BinarySearchTree* p, BinarySearchTree* x);
BinarySearchTree* fixredblack_remove_case1(BinarySearchTree* p, ChildAccessors accessors);
BinarySearchTree* fixredblack_remove_case2(BinarySearchTree* p, ChildAccessors accessors);

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
        (*t)->color = black;
        return;
    }

    ptrBinarySearchTree* cur = t; 
    BinarySearchTree* par = NULL;
    
    while (*cur){
        if (v == bstree_key(*cur))
            return;
        
        par = *cur;
        
        cur = (v > bstree_key(*cur)) 
            ? &((*cur)->right) 
            : &((*cur)->left);
    }

    // Création du nouveau nœud
    *cur = bstree_cons(NULL, NULL, v);
    (*cur)->parent = par;

    // Correction des propriétés rouge-noir
    BinarySearchTree* stop = fixredblack_insert(*cur);
    
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

    ptrBinarySearchTree* link_to_modify = NULL;
    ptrBinarySearchTree substitute = NULL;

    // Determine the parent-child link to modify
    if (!current->parent) {
        // Current node is the root
        link_to_modify = t;
    } else if (current->parent->left == current) {
        // Current node is the left child
        link_to_modify = &(current->parent->left);
    } else {
        // Current node is the right child
        link_to_modify = &(current->parent->right);
    }

    // Case 1: Current node is a leaf
    if (current->left == NULL && current->right == NULL) {
        *link_to_modify = NULL; // Disconnect the node
        substitute = NULL;
    }
    // Case 2: Current node has only the right child
    else if (current->left == NULL) {
        *link_to_modify = current->right;
        if (current->right) {
            current->right->parent = current->parent;
        }
        substitute = current->right;
    }
    // Case 3: Current node has only the left child
    else if (current->right == NULL) {
        *link_to_modify = current->left;
        if (current->left) {
            current->left->parent = current->parent;
        }
        substitute = current->left;
    }
    // Case 4: Current node has both children
    else {
        // Find successor
        substitute = (ptrBinarySearchTree)bstree_successor(current);
        
        // Copy successor's key to current node
        current->key = substitute->key;
        
        // Recursively remove the successor
        // Update link_to_modify and substitute
        if (substitute->parent->left == substitute) {
            substitute->parent->left = substitute->right;
        } else {
            substitute->parent->right = substitute->right;
        }
        
        if (substitute->right) {
            substitute->right->parent = substitute->parent;
        }
        
        // Update substitute to be the right child (which may be NULL)
        substitute = substitute->right;
    }

    // Fix Red-Black tree properties if needed
    if (current->color == black) {
        if (substitute == NULL || substitute->color == black) {
            // Substitute is double black: must fix
            ptrBinarySearchTree subtreeroot = fixredblack_remove(current->parent, substitute);
            
            // Update root if necessary
            if (subtreeroot->parent == NULL) {
                *t = subtreeroot;
            }
        } else {
            // Substitute becomes black
            substitute->color = black;
        }
    }

    // Free the memory
    free(current);
}

void bstree_remove(ptrBinarySearchTree* t, int v) {
  ptrBinarySearchTree current = *t;
  while (current){
    if (current->key == v)
      break;
    current = ( (current->key > v) ? current->left : current->right );
  }
  if (current) {
    bstree_remove_node(t, current);
  }
}

/*------------------------------EXERCICE 6-2------------------------------*/

void leftrotate(BinarySearchTree* x) {
    if (!x || !x->right) return;
    BinarySearchTree* y = x->right;

    x->right = y->left;
    if (y->left) {
        y->left->parent = x;
    }

    y->parent = x->parent;
    if (x->parent) {
        if (x == x->parent->left)
            x->parent->left = y;
        else 
            x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

void rightrotate(BinarySearchTree* y) {
    if (!y || !y->left) return;
    BinarySearchTree* x = y->left;

    y->left = x->right;
    if (!bstree_empty(x->right)) {
        x->right->parent = y;
    }

    x->parent = y->parent;
    if (y->parent) {
        if (y == y->parent->right)
            y->parent->right = x;
        else
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

    // Pass `ChildAccessors` to the first correction case
    ChildAccessors accessors = {access_left, access_right};
    return fixredblack_insert_case1(x, accessors);
}


// Premier cas de correction
BinarySearchTree* fixredblack_insert_case1(ptrBinarySearchTree x, ChildAccessors accessors) {
    assert(!bstree_empty(x) && !bstree_empty(x->parent));

    if (!x || !x->parent || !x->parent->parent)
        return x;

    BinarySearchTree* p = x->parent;
    BinarySearchTree* pp = grandparent(x);

    // Determine uncle using `ChildAccessors`
    const BinarySearchTree* uncle = accessors.left(pp) == p
                                        ? accessors.right(pp)
                                        : accessors.left(pp);

    // If uncle is red
    if (uncle && uncle->color == red) {
        p->color = black;
        ((BinarySearchTree*)uncle)->color = black; // Explicit cast for const-correctness
        pp->color = red;
        return fixredblack_insert(pp);
    }

    // Proceed to next case
    return fixredblack_insert_case2(x, accessors);
}


// Deuxième cas de correction
BinarySearchTree* fixredblack_insert_case2(ptrBinarySearchTree x, ChildAccessors accessors) {
    assert(!bstree_empty(x) && !bstree_empty(x->parent) && !bstree_empty(x->parent->parent));

    BinarySearchTree* p = x->parent;
    BinarySearchTree* pp = grandparent(x);

    // Handle left-right or right-left cases
    if (x == accessors.right(p) && p == accessors.left(pp)) {
        leftrotate(p);
        x = (BinarySearchTree*)accessors.left(x); // Cast for const-correctness
    } else if (x == accessors.left(p) && p == accessors.right(pp)) {
        rightrotate(p);
        x = (BinarySearchTree*)accessors.right(x); // Cast for const-correctness
    }

    // Recompute parent and grandparent
    p = x->parent;
    pp = grandparent(x);

    if (x == accessors.left(p))
        rightrotate(pp);
    else
        leftrotate(pp);

    p->color = black;
    pp->color = red;

    return p;
}


/*-------------------------------EXERCICE 6-5----------------------------------*/

BinarySearchTree* fixredblack_remove(BinarySearchTree* p, BinarySearchTree* x) {
    if (p == NULL) {
        if (x != NULL) x->color = black;
        return x;
    }
    
    // Determine the opposite child based on x's position
    ChildAccessors access = (x == p->left) ? 
        (ChildAccessors){access_right, access_left} : 
        (ChildAccessors){access_left, access_right};
    
    BinarySearchTree* f = (x == p->left) ? p->right : p->left;
    
    if (f->color == black) {
        return fixredblack_remove_case1(p, access);
    } else {
        return fixredblack_remove_case2(p, access);
    }
}

// Removed x parameter from case1 function
BinarySearchTree* fixredblack_remove_case1(BinarySearchTree* p, ChildAccessors access) {
    assert(!bstree_empty(p));
    
    BinarySearchTree* f = (BinarySearchTree*)access.left(p);
    BinarySearchTree* g = (BinarySearchTree*)access.right(f);
    BinarySearchTree* d = (BinarySearchTree*)access.left(f);
    
    if ((g == NULL || g->color == black) && (d == NULL || d->color == black)) {
        // Sous-cas : Les deux fils de f sont noirs
        f->color = red;
        if (p->color == red) p->color = black;
        else return fixredblack_remove(p->parent, p);
    } else if (d != NULL && d->color == red) {
        // Sous-cas : Le fils droit de f est rouge
        if (access.left == access_left) {
            rightrotate(p);
        } else {
            leftrotate(p);
        }
        f->color = p->color;
        p->color = black;
        if (d != NULL) d->color = black;
    } else if (g != NULL && g->color == red) {
        // Sous-cas : Le fils gauche de f est rouge
        if (access.left == access_left) {
            leftrotate(f);
            g->color = black;
            f->color = red;
        } else {
            rightrotate(f);
            g->color = black;
            f->color = red;
        }
        return fixredblack_remove_case1(p, access);
    }
    return p;
}

// Removed x parameter from case2 function
BinarySearchTree* fixredblack_remove_case2(BinarySearchTree* p, ChildAccessors access) {
    assert(!bstree_empty(p));
    
    BinarySearchTree* f = (BinarySearchTree*)access.left(p);
    
    if (access.left == access_left) {
        rightrotate(p);
    } else {
        leftrotate(p);
    }
    
    f->color = black;
    p->color = red;
    
    // Le nouveau frère de x devient noir
    return fixredblack_remove_case1(p, 
        access.left == access_left ? 
        (ChildAccessors){access_right, access_left} : 
        (ChildAccessors){access_left, access_right});
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



