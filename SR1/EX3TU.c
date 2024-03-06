#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

void main(){
    int tubeLG[2], tubeGW[2];
    char com;
    char car;
    if((pipe(tubeLG)==-1)||(pipe(tubeGW)==-1)){
        perror("prbl creation tube");
        exit(1);
    }
    pid_t pid = fork();
    switch(pid){
        case -1:{
            perror("fork");
            exit(2);
        }
        case 0:{
            close(tubeGW[1]);
            close(tubeGW[0]);
            close(tubeLG[0]);
            if(dup2(tubeLG[1],STDOUT_FILENO)==-1){
                perror("Redirection de sortie standard");
                close(tubeLG[1]);
                exit(1);
            }
            execlp("ls","ls","-Ral","HIERARCHIE",NULL);
            perror("ls");
            close(tubeLG[1]);
            exit(2);
        }
    pid_t pid = fork();
    switch(pid){
        case -1:{
            perror("fork");
            
            exit(2);
        }
        case 0:{
            close(tubeGW[0]);
            close(tubeLG[1]);
            if((dup2(tubeLG[0],STDIN_FILENO)==-1)||(dup2(tubeGW[1],STDOUT_FILENO)==-1)){
                perror("Redirection de sortie standard");
                close(tubeGW[1]);
                exit(1);
            }
            execlp("grep","grep","UTILISATEUR",NULL);
            perror("grep");
            exit(2);
        }
        close(tubeLG[1]);
        close(tubeLG[0]);
        close(tubeGW[1]);
        if(dup2(tubeGW[0],STDIN_FILENO)==-1){
            perror("redirection entrée standard");
            exit(3);
        }
        execlp("wc","wc","-l",NULL);
        perror("wc");
        exit(4);
    }
}