#include "bstree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"


/*------------------------  BSTreeType  -----------------------------*/

struct _bstree {
    BinarySearchTree* parent;
    BinarySearchTree* left;
    BinarySearchTree* right;
    int key;
};

typedef BinarySearchTree* (*AccessFunction)(const BinarySearchTree*);

typedef struct {
    AccessFunction first;
    AccessFunction second;
} ChildAccessors;



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
    return t;
}

void freenode(const BinarySearchTree* t, void* n) {
    (void)n;
    free((BinarySearchTree*)t);
}

void bstree_delete(ptrBinarySearchTree* t) {
    bstree_depth_postfix(*t, freenode, NULL);
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
  ptrBinarySearchTree* cur = t;
  BinarySearchTree* par = NULL;
  while(*cur){
    if(bstree_key(*cur)==v){
      return;
    }
    par = *cur;
    if (bstree_key(*cur) > v){
      cur = &((*cur)->left);
    }
    else{
      cur = &((*cur)->right);
    }
  }
  *cur = bstree_cons(NULL,NULL,v);
  (*cur)->parent = par;
}

const BinarySearchTree* bstree_search(const BinarySearchTree* t, int v){
  while(t != NULL){
    if(bstree_key(t) == v){
      return t;
    }
    else if(bstree_key(t)>v){
      t = t->left;
    }
    else{
      t = t->right;
    }
  }
  return NULL;
}


BinarySearchTree* find_next(const BinarySearchTree* x, ChildAccessors access){
  assert(x != NULL);

  BinarySearchTree* y = access.second((BinarySearchTree*)x);
  if (y) {
    while (access.first(y)) {
      y = access.first(y);
    }
  }
  else {
    y = x->parent;
    while (y && x == access.second(y)) {
      x = y;
      y = y->parent;
    }
  }
  return y;
}

const BinarySearchTree* bstree_successor(const BinarySearchTree* x) {
  assert(!bstree_empty(x));
  ChildAccessors access = {bstree_left , bstree_right};
  return find_next(x,access);
}

const BinarySearchTree* bstree_predecessor(const BinarySearchTree* x) {
  assert(!bstree_empty(x));
  ChildAccessors access = {bstree_right , bstree_left};
  return find_next(x,access);
}



void bstree_swap_nodes(ptrBinarySearchTree* tree, ptrBinarySearchTree from, ptrBinarySearchTree to) {
    assert(!bstree_empty(*tree) && !bstree_empty(from) && !bstree_empty(to));
    to->key = from->key;
}

// t -> the tree to remove from, current -> the node to remove
void bstree_remove_node(ptrBinarySearchTree* t, ptrBinarySearchTree current) {
  assert(!bstree_empty(*t) && !bstree_empty(current));
  ptrBinarySearchTree leaf;
  ptrBinarySearchTree *m;
  /* Identify the link to modify */
  if ( ! current->parent )
    m = t;
  else if (current->parent->left == current)
    m = &(current->parent->left);
  else
    m = &(current->parent->right);
  /* Remove node */
  if (current->left == current->right) {
    /* current is a leaf */
    *m = bstree_create();
    free(current);
  } else if (! current->left) {
    /* current has only its right child */
    *m = current->right;
    current->right->parent =current->parent;
    free(current);
  } else if (! current->right) {
    /* current has only its left child */
    *m = current->left;
    current->left->parent =current->parent;
    free(current);
   } else {
     /* current has both childs */
     leaf = (BinarySearchTree*)bstree_successor(current);
     if (leaf) {
       current->key = leaf->key;
     bstree_remove_node(t, leaf);
    }
  }
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

/*------------------------  BSTreeVisitors  -----------------------------*/

void bstree_depth_prefix(const BinarySearchTree* t, OperateFunctor f, void* environment) {
    if(t == NULL){
      return;
    }
    else{
      f(t,environment);
      bstree_depth_prefix(t->left,f,environment);
      bstree_depth_prefix(t->right,f,environment);
    }
}

void bstree_depth_infix(const BinarySearchTree* t, OperateFunctor f, void* environment) {
  if(t == NULL){
    return;
  }
  else{
    bstree_depth_infix(t->left,f,environment);
    f(t,environment);
    bstree_depth_infix(t->right,f,environment);
  }
}

void bstree_depth_postfix(const BinarySearchTree* t, OperateFunctor f, void* environment) {
  if(t == NULL){
    return;
  }
  else{
    bstree_depth_postfix(t->left,f,environment);
    bstree_depth_postfix(t->right,f,environment);
    f(t,environment);
  }
}

void bstree_iterative_breadth(const BinarySearchTree* t, OperateFunctor f, void* environment) {
  if (t == NULL) {
      return;
  }
  Queue* queue = create_queue();
  queue_push(queue, t);
  while (!queue_empty(queue)) {
      BinarySearchTree* current_node = (BinarySearchTree*)queue_top(queue);
      queue_pop(queue);
      f(current_node, environment);
      if (current_node->left != NULL) {
          queue_push(queue, current_node->left);
      }
      if (current_node->right != NULL) {
          queue_push(queue, current_node->right);
      }
  }
  delete_queue(&queue);
}

void bstree_iterative_depth_infix(const BinarySearchTree* t, OperateFunctor f, void* environment) {
  if(t == NULL){
    return;
  }
  else{
    bstree_iterative_depth_infix(t->left,f,environment);
    f(t,environment);
    bstree_iterative_depth_infix(t->right,f,environment);
  }
}

/*------------------------  BSTreeIterator  -----------------------------*/

struct _BSTreeIterator {
    /* the collection the iterator is attached to */
    const BinarySearchTree* collection;
    /* the first element according to the iterator direction */
    const BinarySearchTree* (*begin)(const BinarySearchTree* );
    /* the current element pointed by the iterator */
    const BinarySearchTree* current;
    /* function that goes to the next element according to the iterator direction */
    const BinarySearchTree* (*next)(const BinarySearchTree* );
};

/* minimum element of the collection */
const BinarySearchTree* goto_min(const BinarySearchTree* e) {
	(void)e;
	return NULL;
}

/* maximum element of the collection */
const BinarySearchTree* goto_max(const BinarySearchTree* e) {
	(void)e;
	return NULL;
}

/* constructor */
BSTreeIterator* bstree_iterator_create(const BinarySearchTree* collection, IteratorDirection direction) {
	(void)collection; (void)direction;
	return NULL;
}

/* destructor */
void bstree_iterator_delete(ptrBSTreeIterator* i) {
    free(*i);
    *i = NULL;
}

BSTreeIterator* bstree_iterator_begin(BSTreeIterator* i) {
    i->current = i->begin(i->collection);
    return i;
}

bool bstree_iterator_end(const BSTreeIterator* i) {
    return i->current == NULL;
}

BSTreeIterator* bstree_iterator_next(BSTreeIterator* i) {
    i->current = i->next(i->current);
    return i;
}

const BinarySearchTree* bstree_iterator_value(const BSTreeIterator* i) {
    return i->current;
}
