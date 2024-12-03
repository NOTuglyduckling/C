#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "skiplist.h"
#include "rng.h"

/**
 *	@defgroup SkipListTest Test program for SkipList Implantation
 *  @brief Definition of the SkipList type and operators
 *  @{
 */

/** Print a usage message for this program.
 
 @code{.unparsed}
 $skiplisttest -id num
 where id is :
 	c : construct and print the skiplist with data read from file ../Test/test_files/construct_num.txt
 	s : construct the skiplist with data read from file ../Test/test_files/construct_num.txt and search elements from file test_files/search_num..txt
 		Print statistics about the searches.
 	i : construct the skiplist with data read from file ../Test/test_files/construct_num.txt and search, using an iterator, elements read from file test_files/search_num.txt
 		Print statistics about the searches.
 	r : construct the skiplist with data read from file test_files/construct_num.txt, remove values read from file test_files/remove_num.txt and print the list in reverse order
 
 and num is the file number for input.
 @endcode
 */
void usage(const char *command) {
	printf("usage : %s -id num\n", command);
	printf("where id is :\n");
	printf("\tc : construct and print the skiplist with data read from file test_files/construct_num.txt\n");
	printf("\ts : construct the skiplist with data read from file test_files/construct_num.txt and search elements from file test_files/search_num..txt\n\t\tPrint statistics about the searches.\n");
	printf("\ti : construct the skiplist with data read from file test_files/construct_num.txt and search, using an iterator, elements read from file test_files/search_num.txt\n\t\tPrint statistics about the searches.\n");
	printf("\tr : construct the skiplist with data read from file test_files/construct_num.txt, remove values read from file test_files/remove_num.txt and print the list in reverse order\n");
	printf("and num is the file number for input\n");
}

/** Return the filename associated with the action to perform and the number of a test.
 
 @param action Describe the action associated with the file to load. Must be one of "construct" "search" or "remove"
 @param filenumber Number of the file to load. Corresponds to the number of the test to perform.
 @return fileame  in a relative form
 
 @code{.c}
 char *filename = gettestfilename("construct", 2);
 @endcode
 will set filename to @code{.c}"../Test/test_files/construct_2.txt"@endcode
 
 */
char *gettestfilename(const char *action, int numtest) {
	const char *testdir ="../Test/test_files/";
	int l = strlen(testdir) + strlen(action) + strlen("_xxx.txt");
	char *name = malloc(l*sizeof(char));
	sprintf(name, "%s%s_%d.txt", testdir, action, numtest);
	return name;
}

int read_int(FILE* input) {
  int v;
  int r = fscanf(input,"%d", &v);
  if (r == 1) {
    return v;
  }
  perror("Unable to read int from input file\n");
  abort();
}

unsigned int read_uint(FILE* input) {
  unsigned int v;
  int r = fscanf(input,"%u", &v);
  if (r == 1) {
    return v;
  }
  perror("Unable to read uint from input file\n");
  abort();
}

/** Build a list corresponding to the given file number.
 */
SkipList* buildlist(int num) {
	SkipList* d;
	FILE *input;
	
	char *constructfromfile = gettestfilename("construct", num);
	input = fopen(constructfromfile, "r");
	if (input!=NULL) {
		d = skiplist_create(read_uint(input));
		unsigned int nb_values = read_uint(input);
		for (unsigned int i=0;i< nb_values; ++i) {
			d = skiplist_insert(d, read_int(input));
		}
	} else {
		printf("Unable to open file %s\n", constructfromfile);
		free(constructfromfile);
		exit (1);
	}
	free(constructfromfile);
	fclose(input);
	return d;
}

/*----------------------------------------------------------------------------------------------*/

/** Exercice 1.
 	Programming and test of skiplist construction.
 */
void print_value(int value, void* user_data) {
    (void)user_data;
    printf("%d ", value);
}

void skiplist_print(const SkipList* d) {
    printf("Skiplist (%u)\n", skiplist_size(d));
    skiplist_map(d, print_value, NULL);
    printf("\n");
}

void test_construction(int num) {
    SkipList* skiplist = buildlist(num);
    skiplist_print(skiplist);
    skiplist_delete(&skiplist);
}

/** Exercice 2.
 Programming and test of skiplist search operator.
 */
void test_search(int num) {
    SkipList *skiplist = buildlist(num);

    char *searchfile = gettestfilename("search", num);
    FILE *file = fopen(searchfile, "r");
    if (!file) {
        printf("Error opening search file %s\n", searchfile);
        free(searchfile);
        return;
    }
    
    unsigned int num_searches = read_uint(file); // Nombre de valeurs à chercher
    int found_count = 0, not_found_count = 0;
    unsigned int min_operations = skiplist_size(skiplist), max_operations = 0, total_operations = 0;
    unsigned int operations;
    for (unsigned int i = 0; i < num_searches; i++) {
        int value_to_search = read_int(file);
        
		if (skiplist_search(skiplist, value_to_search,&operations)) {
			printf("%d -> true\n", value_to_search);
			found_count++;
		} else {
			printf("%d -> false\n", value_to_search);
			not_found_count++;
		}

        total_operations += operations;
        if (operations < min_operations) min_operations = operations;
        if (operations > max_operations) max_operations = operations;
    }
    free(searchfile);
    fclose(file);

    unsigned int mean_operations = total_operations / num_searches;

    // Afficher les statistiques
    printf("Statistics : \n");
    printf("\tSize of the list : %u\n", skiplist_size(skiplist));
    printf("Search %u values :\n", num_searches);
    printf("\tFound %d\n", found_count);
    printf("\tNot found %d\n", not_found_count);
    printf("\tMin number of operations : %d\n", min_operations);
    printf("\tMax number of operations : %d\n", max_operations);
    printf("\tMean number of operations : %d\n", mean_operations); 
    
    skiplist_delete(&skiplist);
}


/** Exercice 3.
 Programming and test of naïve search operator using iterators.
 */
void test_search_iterator(int num) {
    SkipList* skiplist = buildlist(num);

    char *search_filename = gettestfilename("search", num);
    FILE* search_file = fopen(search_filename, "r");
    if (search_file == NULL) {
        perror("Unable to open search file");
        free(search_filename);
        return;
    }

    unsigned int num_searches = read_uint(search_file);
    int found_count = 0, not_found_count = 0;
    unsigned int min_operations = skiplist_size(skiplist), max_operations = 0, total_operations = 0;
    unsigned int operations;
    bool found;
    SkipListIterator* iter;
    for (unsigned int i = 0; i < num_searches; i++) {
        int value = read_int(search_file);
        iter = skiplist_iterator_create(skiplist, FORWARD_ITERATOR);

        operations = 0;
        found=false;

        for (iter = skiplist_iterator_begin(iter); !skiplist_iterator_end(iter); iter = skiplist_iterator_next(iter)) {
            operations++; 
            if (skiplist_iterator_value(iter) == value) {
                found=true;
                break;
		    }
        }
        if (found){
            printf("%d -> true\n", value);
			found_count++;
        } else {
            printf("%d -> false\n", value);
		    not_found_count++;
        }
        
        total_operations += operations;
        if (operations < min_operations) min_operations = operations;
        if (operations > max_operations) max_operations = operations;

        skiplist_iterator_delete(&iter);
    }

    fclose(search_file);
    free(search_filename);

    unsigned int mean_operations = total_operations / num_searches;

    // Afficher les statistiques
    printf("Statistics : \n");
    printf("\tSize of the list : %u\n", skiplist_size(skiplist));
    printf("Search %u values :\n", num_searches);
    printf("\tFound %d\n", found_count);
    printf("\tNot found %d\n", not_found_count);
    printf("\tMin number of operations : %d\n", min_operations);
    printf("\tMax number of operations : %d\n", max_operations);
    printf("\tMean number of operations : %d\n", mean_operations); 

    skiplist_delete(&skiplist);
}

/** Exercice 4.
 Programming and test of skiplist remove operator.
 */
void test_remove(int num) {
    SkipList* skiplist = buildlist(num);

    char* remove_file = gettestfilename("remove", num);
    FILE* input = fopen(remove_file, "r");
    if (!input) {
        printf("Unable to open file %s\n", remove_file);
        free(remove_file);
        return;
    }

    unsigned int num_removals = read_uint(input);
    for (unsigned int i = 0; i < num_removals; i++) {
        int value_to_remove = read_int(input);
        skiplist_remove(skiplist, value_to_remove);
    }

    fclose(input);
    free(remove_file);

    printf("Skiplist (%d)\n", skiplist_size(skiplist));

    SkipListIterator* it = skiplist_iterator_create(skiplist, BACKWARD_ITERATOR);

    for (it = skiplist_iterator_begin(it); !skiplist_iterator_end(it); it = skiplist_iterator_next(it)) {
        if (skiplist_iterator_value(it) != -1)
            printf("%d ", skiplist_iterator_value(it));
    }
    printf("\n");

    skiplist_iterator_delete(&it);
    skiplist_delete(&skiplist);
}



/** Function you ca use to generate dataset for testing.
 */
void generate(int nbvalues);


int main(int argc, const char *argv[]){
	if (argc < 3) {
		usage(argv[0]);
		return 1;
	}
	switch (argv[1][1]) {
		case 'c' :
			test_construction(atoi(argv[2]));
			break;
		case 's' :
			test_search(atoi(argv[2]));
			break;
		case 'i' :
			test_search_iterator(atoi(argv[2]));
			break;
		case 'r' :
			test_remove(atoi(argv[2]));
			break;
		case 'g' :
			generate(atoi(argv[2]));
			break;
		default :
			usage(argv[0]);
			return 1;
	}
	return 0;
}


/* Generates a set of test files for a given number of value. */

void generate(int nbvalues){
	FILE *output;
	int depth;
	int maxvalue;
	output = fopen("construct.txt", "w");
	srand(nbvalues);
	depth = rand()%16;
	maxvalue = rand()%10 * nbvalues;
	fprintf(output, "%d\n%d\n", depth, nbvalues);
	for (int i=0; i< nbvalues; ++i) {
		fprintf(output, "%d\n", rand()%maxvalue);
	}
	fclose(output);
	output = fopen("search.txt", "w");
	srand(rand());
	nbvalues *= depth/4;
	fprintf(output, "%d\n", nbvalues);
	for (int i=0; i< nbvalues; ++i) {
		fprintf(output, "%d\n", rand()%maxvalue);
	}
	fclose(output);
}


/** @} */

/**
 @mainpage
 @section TP45 Implantation du TAD Listes à raccourci.
 @subsection SkipLists SkipLists
 Les listes à raccourcis, nommées SkipLists, sont une alternative aux arbres de
 recherche équilibrés que nous verrons dans la suite du cours.
 
 Inventées par William Pugh en 1902, elles reposent sur une structure de données
 linéaire construite de manière probabiliste.
 Les opérations de dictionnaire définies sur les listes à raccourcis sont simples à
 programmer, élégantes, et l’aspect probabiliste permet de démontrer une complexité
 moyenne en O(log(N)) au lieu des O(N) inhérents aux structures linéaires.
 
 Toutefois, l’aspect probabiliste de la structure de données ne permet pas d’assurer
 la complexité en pire cas, comme on pourra le faire sur les arbres équilibrés, mais
 donne, en pratique, une structure extrêmement efficace (la probabilité de construire
 une structure non efficace étant très faible). Nous ne ferons pas de démonstration
 ici, et il ne vous est pas demandé d’en effectuer une, les étudiants curieux
 pourront se rapporter à leur cours de complexité et à l’analyse fournie dans
 l’article de William Pugh, accessible sur Moodle et sur la <a href="https://dl.acm.org/citation.cfm?id=78977"> bibliothèque ACM</a>.
 
 @note \anchor WP-skiplist-1990 William Pugh, <a href="https://dl.acm.org/citation.cfm?id=78977">Skip lists : a probabilistic alternative to balanced trees</a> in Communications of the ACM, June 1990, 33(6) 668-676
 
 @subsection Objectifs Objectifs du TP
 - Implantation du TAD SkipList
 - Implantation d'un itérateur sur une SkipList
 - Comparaison empirique des opérations de dictionnaires sur les SkipList et en utilisant un itérateur.
 
 */
