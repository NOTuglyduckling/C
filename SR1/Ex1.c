#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main (int argc, char * argv[]){
    if (argc != 2){
        fprint(stderr,"Usage : %s ENV\n",argv[0]);
        exit(1);
    } 
    char* val = getenv(argv[1]);
    if (val == NULL){
        fprintf(stderr,"%s n'est pas une variable d'environement\n",argv[1]);
        exit(2);
    }
    printf("%s\n",val);
    return 0;
}