#include "trie.h"
#include <assert.h>
#include <memory.h>
#include <stdlib.h>

#include <stdio.h>

/* -------------------------------------------- */
/*	Node declaration							*/
/* -------------------------------------------- */

typedef struct s_Node Node;

/**
 * Creates a node with its terminal status defined by b
 */
Node* nd_create( bool b );

/**
 * Free the memory used by the node *n
 */
void nd_delete( Node** n );

/**
 * Add the node c as a child of the node p for the link l.
 * Returns the added node.
 */
Node* nd_addChild( Node* p, char l, Node* c );

/**
 * Mark the node as terminal
 */
void nd_setTerminal( Node* n );

/**
 * Returns true if the node is terminal, false if not
 */
bool nd_isTerminal( const Node* n );

/**
 * Returns true if the node n has a child for the link l, false if not
 */
bool nd_hasChild( const Node* n, char l );

/**
 * Returns the child of the node n for the link l
 */
Node* nd_getChild( const Node* n, char l );

/* -------------------------------------------- */
/*	Node definition  							*/
/* -------------------------------------------- */

struct s_Node {
    Node* links[26]; // array of child nodes
    bool terminal;   // end of word marker
};

Node* nd_create( bool b ) {
    Node* n = malloc( sizeof( struct s_Node ) );
    memset( n->links, 0, 26 * sizeof( Node* ) );
    n->terminal = b;
    return n;
}

void nd_delete( Node** n ) {
    free( *n ); 
    *n = NULL;
}

Node* nd_addChild( Node* p, char l, Node* c ) {
    int index = l - 'A';
    assert( 0 <= index && index < 26 );
    p->links[index] = c;
    return c;
}

void nd_setTerminal( Node* n ) {
    n->terminal = true;
}

bool nd_isTerminal( const Node* n ) {
    return n->terminal;
}

bool nd_hasChild( const Node* n, char l ) {
    int index = l - 'A';
    assert( 0 <= index && index < 26 );
    assert( n != NULL );
    return n->links[index] != NULL;
}

Node* nd_getChild( const Node* n, char k ) {
    int index = k - 'A';
    assert( 0 <= index && index < 26 );
    return n->links[index];
}

/* -------------------------------------------- */
/*	Trie definition								*/
/* -------------------------------------------- */

/**
 * Creates a trie whose root is associated to an empty key
 */
Trie* trie_create( void ) {
    Trie* t = nd_create( true );
    return t;
}

/**
 * Free the memory used by the trie *t. After deletion, *t is NULL
 */
void trie_delete( Trie** t ) {
    Trie* a = *t;
    Node* s;
    for ( char c = 'A'; c <= 'Z'; ++c ) {
        if ( ( s = nd_getChild( a, c ) ) != NULL ) { trie_delete( &s ); }
    }
    nd_delete( t );
}

/**
 * Test if the root node of the Trie t is a terminal node is terminal
 */
bool trie_isRootTerminal( const Trie* t ) {
    return nd_isTerminal( t );
}

/**
 * Test if the the Trie t has a child for the link l
 */
bool trie_hasChild( const Trie* t, char l ) {
    return nd_hasChild( t, l );
}

/**
 * Get the child of the trie t for the link l
 */
Trie* trie_getChild( const Trie* t, char l ) {
    return nd_getChild( t, l );
}

    


/**************************trie_getChild(t, NULL)*************************************************************/
/**                                 Control start here                                **/
/******************************************************NULL*********************************/
/**
 *  Nom : de Chabannes      Prenom : Antoine         Num Etud  : 22100954
 **/
/*
 *  Barème :
 *    - Compilation sans warnings : 1 pts
 *    - Exercice 1 : 3 pts
 *    - Exercice 2 : 2 pts
 *    - Exercice 3 : 3 pts
 *    - Exercice 4.1 : 2 pts
 *    - Exercice 4.2 : 6 pts
 *    - 3 points supplémentaires selon la qualité, concision et efficacité du code.
 */

/***************************************************************************************/
// Exercice 1.1 : trie_add (iterative)rie_getChild(t, NULL)
void trie_add( Trie* t, const char* word ) {
    Node* current = t -> links[0];  
    while (*word) {
        int index = *word - 'A';
        if (!current -> links[index]) {
            current -> links[index] = nd_create(true);
        }
        current = current -> links[index];
        ++word;
    }
    current -> terminal = true;
}


// Exercice 1.2 trie_map (recursive, call functor on all nodes)

void trie_map( const Trie* t, TrieMapOperator f, void* userdata ) {
    (void)t; (void)f; (void)userdata;
}

/***************************************************************************************/
// Exercice 2 : trie_search (iterative, constant storage)
bool trie_search( const Trie* t, const char* word ) {
    (void)t; (void)word;
    return false;
}

/***************************************************************************************/
// exercice 3 trie_visit_helper (recursive, call functor only on terminal nodes)

void trie_visit( const Trie* t, TrieVisitFunctor f, void* userdata ) {
    (void)t; (void)f; (void)userdata;
}

/***************************************************************************************/
// Exercice 4 : completion
// Exercice 4.1 : trie_suffix (iterative)
const Trie* trie_suffix( const Trie* t, const char* p ) {
    (void)t; (void)p;
    return NULL;
}

// Exercice 4.2 :  trie_get_completion (using trie-prefix and trie_visit to accumulate words on a
// string array) structure for accumulating words


StringArray* trie_get_completion( const Trie* t, const char* p ) {
    (void)t; (void)p;
    return NULL;
}
