#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    int pid = fork();
    if (pid==-1){
        perror("Fork not created");
        exit (1);
    }
    if (pid==0){
        execl("/bin/ls","ls","-al",NULL);
        perror("ls");
        exit(1);
    }
    wait(NULL);
    execl("/bin/date","date",NULL);
    perror("date");
    exit(0);
}