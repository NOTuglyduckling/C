
#define _GNU_SOURCE
#include "trie.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * Export du trie au format dot
 */
void printNode( const Trie* n, char key, void* userdata ) {
    FILE* file  = (FILE*)userdata;
    int nbchild = 0;
    Trie* s;

    // First print the node key
    fprintf( file, "\tn0x%p [", (void*)n );
    if ( key == '\0' ) { fprintf( file, " label=\"{root|" ); }
    else {
        if ( trie_isRootTerminal( n ) ) {
            fprintf( file, "style=filled, fillcolor=grey, label=\"{" );
        }
        else { fprintf( file, " label=\"{" ); }
    }
    // print the port for linking
    for ( char c = 'A'; c <= 'Z'; ++c ) {
        if ( trie_getChild( n, c ) != NULL ) {
            if ( nbchild == 0 ) { fprintf( file, "{" ); }
            else { fprintf( file, "|" ); }
            fprintf( file, "<%c>(%c)", c, c );
            ++nbchild;
        }
    }
    if ( nbchild != 0 ) { fprintf( file, "}" ); }
    fprintf( file, "}\"];\n" );

    // then define links
    for ( char c = 'A'; c <= 'Z'; ++c ) {
        if ( ( s = trie_getChild( n, c ) ) != NULL ) {
            fprintf( file, "\tn0x%p:%c:s -> n0x%p:n\n", (void*)n, c, (void*)s );
        }
    }
}

void trie_exportDot( Trie* t, const char* filename ) {
    char dotname[strlen( filename ) + 1];
    strcpy( dotname, filename );
    strcpy( dotname + strlen( dotname ) - 3, "dot" );
    printf( "Exporting the trie to %s ... ", dotname );
    FILE* output = fopen( dotname, "w" );
    fprintf( output, "digraph Trie {\n\tgraph [];\n\tnode [shape = record];\n\n" );
    trie_map( t, printNode, output );
    fprintf( output, "\n}\n" );
    fclose( output );
    printf( " Done\n" );
}

/**
 * Construction d'un Trie depuis un fichier de mots
 */
int read_word( char** word, size_t* size, FILE* fp ) {
    int ret = getline( word, size, fp );
    if ( ret != -1 ) {
        char* pos;
        // skip \n if readed
        if ( ( pos = strchr( *word, '\n' ) ) != NULL ) *pos = '\0';
        char* ptr = *word;
        while ( *ptr ) {
            // ignore undef char and replace by 'A'
            if ( !isascii( (int)*ptr ) || !isalpha( *ptr ) ) { *ptr = 'A'; }
            // ensure upper case
            *ptr = toupper( *ptr );
            ptr++;
        }
    }
    return ret;
}

void init_from_file( Trie* data, FILE* file ) {
    printf( "Building Trie ... " );
    char* word;
    word        = malloc( 256 );
    size_t size = 256;
    while ( read_word( &word, &size, file ) != -1 ) {
        printf( "%s, ", word );
        trie_add( data, word );
    }
    printf( "\b \b\b \b\n" );
    free( word );
}

void search_from_file( Trie* data, FILE* file ) {
    char* word;
    word        = malloc( 256 );
    size_t size = 256;
    while ( read_word( &word, &size, file ) != -1 ) {
        bool r = trie_search( data, word );
        printf( "\t%s : %s\n", word, ( r ? "true" : "false" ) );
    }
    free( word );
}

void print_sa( const char* word, void* userdata ) {
    FILE* f = (FILE*)userdata;
    fprintf( f, "%s, ", word );
}

void search_by_prefix( Trie* data, FILE* file ) {
    char* word;
    word        = malloc( 256 );
    size_t size = 256;
    while ( read_word( &word, &size, file ) != -1 ) {
        printf( "\tPrefix %s : ", word );
        StringArray* words = trie_get_completion( data, word );
        sa_visit( words, print_sa, stdout );
        printf( "\b \b\b \b\n" );
        sa_delete( &words );
    }
    free( word );
}

void print_word( const char* word, void* userdata ) {
    (void)userdata;
    printf( "%s ", word );
}
/**
 * main program
 */
int main( int argc, char** argv ) {

    if ( argc < 2 ) {
        fprintf( stderr, "usage : %s filename\n", argv[0] );
        return 1;
    }

    FILE* input = fopen( argv[1], "r" );
    if ( !input ) {
        perror( argv[1] );
        return 1;
    }

    // Build the Trie
    Trie* theTrie = trie_create();
    init_from_file( theTrie, input );
    fclose( input );
    // export the Trie to dot file
    trie_exportDot( theTrie, argv[1] );

    // search for words
    {
        char fileSearch[strlen( argv[1] ) + 8];
        strcpy( fileSearch, argv[1] );
        char* separator = strrchr( fileSearch, '/' ) + 1;
        if ( separator ) {
            char basename[strlen( separator ) + 8];
            strcpy( basename, "search_" );
            strcat( basename, separator );
            *separator = 0;
            strcat( fileSearch, basename );
        }
        else {
            strcpy( fileSearch, "search_" );
            strcat( fileSearch, argv[1] );
        }
        printf( "Searching words into the dictionary : \n" );

        FILE* searchInput = fopen( fileSearch, "r" );
        search_from_file( theTrie, searchInput );
    }
    // visit words
    {
        printf( "Words on trie : " );
        trie_visit( theTrie, print_word, NULL );
        printf( "\n" );
    }
    // search for prefix
    {
        char filePrefix[strlen( argv[1] ) + 5];
        strcpy( filePrefix, argv[1] );
        char* separator = strrchr( filePrefix, '/' ) + 1;
        if ( separator ) {
            char basename[strlen( separator ) + 5];
            strcpy( basename, "pref_" );
            strcat( basename, separator );
            *separator = 0;
            strcat( filePrefix, basename );
        }
        else {
            strcpy( filePrefix, "search_" );
            strcat( filePrefix, argv[1] );
        }
        printf( "Finding words by prefix\n" );

        FILE* searchInput = fopen( filePrefix, "r" );
        search_by_prefix( theTrie, searchInput );
    }

    trie_delete( &theTrie );
    return 0;
}