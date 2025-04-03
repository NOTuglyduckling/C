#include "stringarray.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct s_Array {
    char** data;
    size_t size;
    size_t capacity;
};

bool sa_resize( StringArray* a, size_t cap ) {
    char** newData = realloc( a->data, sizeof( char* ) * cap );
    if ( newData == NULL ) { return false; }
    a->data = newData;
    memset( a->data + a->capacity, 0, cap - a->capacity );
    a->capacity = cap;
    return true;
}

StringArray* sa_create() {
    StringArray* a = malloc( sizeof( StringArray ) );
    a->size        = 0;
    a->capacity    = 1;
    a->data        = calloc( a->capacity, sizeof( char* ) );
    return a;
}

void sa_delete( StringArray** a ) {
    StringArray* t = *a;
    if ( t ) {
        for ( size_t i = 0; i < t->size; ++i ) {
            free( t->data[i] );
        }
        free( t->data );
        free( t );
        *a = NULL;
    }
}

bool sa_add( StringArray* a, const char* s ) {
    size_t index = a->size;
    if ( index == a->capacity ) {
        if ( !sa_resize( a, 2 * a->capacity ) ) { return false; }
    }
    size_t size    = strlen( s ) + 1;
    a->data[index] = malloc( size );
    strncpy( a->data[index], s, size );
    ++a->size;
    return true;
}

int sa_size( const StringArray* a ) {
    return a->size;
}

const char* sa_getString( const StringArray* a, size_t p ) {
    assert( p < a->size );
    return a->data[p % a->size];
}

int cstring_cmp( const void* a, const void* b ) {
    const char** ia = (const char**)a;
    const char** ib = (const char**)b;
    return strcmp( *ia, *ib );
}

void sa_sort( StringArray* a ) {
    qsort( a->data, a->size, sizeof( char* ), cstring_cmp );
}

void sa_visit( StringArray* a, VisitFunctor f, void* userData ) {
    if ( a ) {
        for ( size_t i = 0; i < a->size; ++i ) {
            f( a->data[i], userData );
        }
    }
}
