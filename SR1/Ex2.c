#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main (void){
    pid_t pid=fork() ;
    if (pid==-1){
        perror("fork not created");
        exit (1);
    } else if (pid){
        printf("[père]%d\n",getpid());
    }else{
        printf("[fils]%d%d\n",getppid(),getpid());
    }
}