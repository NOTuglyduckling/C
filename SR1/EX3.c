#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <sys/time.h>


void Afficher(int i,int succ){
    for (int n=0; n<1; n++){
        printf("[%d] mon pid %d, mon succ %d\n", i,getpid(),succ);
    }
}

int main(){
    int i;
    int succ = getpid();
    pid_t pid;
    for (i=4; i>0;i--){
        pid=fork();
        if (pid==-1){
            perror("fork not created");
            exit (1);
        } else if (!pid){
            Afficher(i,succ);
            exit(i);
        }
        succ=pid;
    }
    Afficher(i,succ);
    int retour;
    while ((waitpid(pid,&retour,0))!=-1){
        printf("[père] mon fils %d, numero %d est terminé\n", pid,WEXITSTATUS(retour));
    }
    printf("[père] j'ai fini\n");
    exit (0);
}

