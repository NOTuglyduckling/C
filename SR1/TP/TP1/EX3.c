#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "decoupe.h"

int main(int argc, char* argv[]){
    if (argc<=1){
        fprintf(stderr,"Usage %s fonctions a exécuter..;",argv[0]);
        exit(1);
    }
    pid_t pid;
    int i;
    int ppid=getpid();
    for (i=1;i<argc;i++){
        char* pMots[NBMOTSMAX+1];
        int pid = fork();
        if (pid==-1){
            perror("Fork not created");
            exit (1);
        }
        if (pid==0){
            printf("[%d] Je lance %s.\n",getpid(),argv[i]);
            decoupe(argv[i],pMots);
            execvp(pMots[0],pMots);
            perror(argv[i]);
            exit(1);
        }
        printf("[%d] J'ai délégué %s à %d. J'attends sa fin...\n",ppid,argv[i],pid);

    }
    int status;
    for (i=1;i<argc;i++){
        pid =wait(&status);
        printf("[%d] %d terminé.\n", ppid, pid);
    }
    printf("[%d] J'ai fini.\n",ppid);
    exit(0);
}