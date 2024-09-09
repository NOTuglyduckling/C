#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>


int main (int argc, char * argv[]){
    if (argc == 1){
        fprintf(stderr,"Usage : %s entiers\n",argv[0]);
        exit(1);
    }
    char * endptr;
    for (int i =1;i<argc;i++){
        long int val = strtol(argv[i],&endptr,10);
        if (errno!=0){
            perror("Echec conversion");
        else {
            if (endptr == argv[i]){
                printf("%s n'est pas valide \n", argv[i]);
            } else{
                printf("Enter %d : %ld \n", i, val);
            }
        }
            exit(2);
        }
        printf("%ld\n",val);
    }
    exit(0);
}