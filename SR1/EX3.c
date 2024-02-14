#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#define N 10;

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
    while ((pid=wait(&retour))!=-1){
        printf("[père] mon fils %d, numero %d est terminé\n", pid,WEXITSTATUS(retour));
    }
    printf("[père] j'ai fini\n");
    exit (0);
}

void Afficher(int i,int succ){
    for (int i=0; i<N;i++){
        printf("[%d] mon pid %d, mon succ %d", n,getpid(),succ);
    }
}