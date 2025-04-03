/*-----------------------------------------------------------------*/
/*
 Licence Informatique - Algorithmique 3
 Mathias Paulin (Mathias.Paulin@irit.fr)

 Interface pour l'implantation du TAD Trie.

 */
/*-----------------------------------------------------------------*/
#ifndef __TRIE_H__
#define __TRIE_H__
#include <stdbool.h>
#include <stdio.h>

#include "stringarray.h"

/**
 * Opaque definition of the type Trie
 */
typedef struct s_Node Trie;

/**
 * Constructor : builds an empty Trie
 */
Trie* trie_create( void );

/**
 * Delete the trie and free the used memory
 */
void trie_delete( Trie** t );

/**
 * Test if the root node of the Trie t is a terminal node is terminal
 */
bool trie_isRootTerminal( const Trie* t );

/**
 * Test if the the Trie t has a child with key k
 */
bool trie_hasChild( const Trie* t, char k );

/**
 * Get the child with key k from the Trie t
 */
Trie* trie_getChild( const Trie* t, char k );

/*********************************************************************/
/**
 * Add a word in a Trie
 */
void trie_add( Trie* t, const char* word );
/**
 * Functor type for trie_map
 * Will be called with the (sub)-trie root r, the letter key assciated with this root
 * and user provided environment
 */
typedef void ( *TrieMapOperator )( const Trie* r, char key, void* userdata );
/**
 * Map the given functor on every node of the Trie
 */
void trie_map( const Trie* t, TrieMapOperator f, void* userdata );

/**
 * Search the Trie for a given world.
 * Returns true if the world exists in the tree (a terminal node is associated
 * with the word), false else.
 */
bool trie_search( const Trie* t, const char* word );

/**
 * Functor type for trie_visit
 * Will be called with the key asociated with every terminal nodes on the Trie
 * and user provided environment
 */
typedef void ( *TrieVisitFunctor )( const char* word, void* userdata );
/**
 * Map the given functor on every terminal node of the Trie
 */
void trie_visit( const Trie* t, TrieVisitFunctor f, void* userdata );

/**
 * Return the root of the sub-Trie whose prefix is p in the Trie t
 */
const Trie* trie_suffix( const Trie* t, const char* p );

/**
 * Return a StringArray with all words of the Trie having the prefix p
 */
StringArray* trie_get_completion( const Trie* t, const char* p );

#endif // __TRIE_H__
