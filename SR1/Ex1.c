#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main (int argc, char * argv[]){
<<<<<<< Updated upstream
    if (argc < 2){
=======
    if (argc != 2){
>>>>>>> Stashed changes
        fprintf(stderr,"Usage : %s ENV\n",argv[0]);
        exit(1);
    }
    for (int i = 1; i<argc;i++){
    printf("%s\n",argv[i]);
    }
    exit(0);
}