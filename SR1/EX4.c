#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void){
    int pid=fork();
    if (pid==-1){
        perror("fork not created");
        exit (1);}
    if (pid==0){ /* on est dans le fils */
        execl("/bin/ls", "ls", "-l", NULL);
        perror("ls");
        exit(1);}
    /* on est dans le père */
    wait(NULL); /* le père attend la fin du fils */
    pid=fork();
    if (pid==-1){
        perror("fork not created");
        exit (1);}
    if (pid==0){ /* on est dans le fils */
        execl("/bin/pwd", "pwd", NULL);
        perror("pwd");}
    wait(NULL); /* le père attend la fin du fils */
    exit(0); /* terminaison du père */
} /* (le fils n'exécute pas cette instruction) *