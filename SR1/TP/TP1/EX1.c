#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int info(){
    int pid = getpid();
    int ppid = getppid();
    printf("Pid du processus : %d, Pid du père : %d\n",pid, ppid);
    printf("PGRP : %d\n", getpgrp());
    printf("UID : %d\n", getuid());
    printf("GID : %d\n", getgid());
        if (getlogin() == NULL) {
        perror("Erreur lors de la récupération du nom d'utilisateur");
        exit(1);
    }

    printf("Nom d'utilisateur : %s\n", getlogin());
    return pid;
}

int main(){
    int pid = fork();
    if (pid==-1){
        perror("Fork not created");
        exit (1);
    }
    if (!pid){
        int pid = info();
        exit (pid);
    }
    int inf;
    wait(&inf);
    printf("[père] mon fils de pid %d, à fini.\n",WEXITSTATUS(inf));
    exit (90);
}