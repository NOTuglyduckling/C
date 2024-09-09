#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int main(void){
    pid_t pid=fork();
    if (pid==-1){
        perror("fork not created");
        exit (1);}
    if (pid==0){
        execlp("ls", "ls", "-l", NULL);
        perror("ls");
        exit(1);}
    wait(NULL);
    pid=fork();
    if (pid==-1){
        perror("fork not created");
        exit (1);}
    if (pid==0){
        execlp("pwd", "pwd", NULL);
        perror("pwd");}
    wait(NULL);
    exit(0);
}