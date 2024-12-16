/*-----------------------------------------------------------------*/
/*
 Licence Informatique - Algorithmique 3
 Mathias Paulin (Mathias.Paulin@irit.fr)

 Interface pour l'implantation du TAD StaticSearchTree.

 */
/*-----------------------------------------------------------------*/
#ifndef __STRING_ARRAY_H__
#define __STRING_ARRAY_H__
#include <stdbool.h>
#include <stdio.h>

/**
 * \brief Management of a dynamic array of C-style strings
 */
typedef struct s_Array StringArray;

/**
 * Construct a string array
 */
StringArray* sa_create();

/**
 * delete the array, deleting all strings in the array
 */
void sa_delete( StringArray** a );

/**
 * Add a string to the array
 * \param a the string array
 * \param s the string to add
 * \return true if the string was added to the array, false if not
 */
bool sa_add( StringArray* a, const char* s );

/**
 * Get the number of strings in a string array
 */
int sa_size( const StringArray* a );

/**
 * Get the string at the given index of strings in a string array
 */
const char* sa_getString( const StringArray* a, size_t p );

/**
 * Sort the array in alphabetical order
 */
void sa_sort( StringArray* a );

/** Functor with user data to be used with the string array visitor.
 *  This functor receive as argument the visited string and an opaque
 *  pointer to user provided data.
 */
typedef void ( *VisitFunctor )( const char*, void* );

/**
 * Visit the string array and apply the given functor on each visited string
 */
void sa_visit( StringArray* a, VisitFunctor f, void* userData );

#endif // __STRING_ARRAY_H__
