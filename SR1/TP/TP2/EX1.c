#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h> 

void ecrire_dans_stdout (char nom_fich[]){
    ssize_t cread;
    char buffer[512];
    int fs = open(nom_fich, O_RDONLY);
    if (fs==-1){
        perror(nom_fich);
        exit(1);}
    while ((cread=read(fs,buffer,sizeof(buffer)))>0){
        if (write(STDOUT_FILENO,buffer,cread)!=cread){
            perror("STDOUT");
            close(fs);
            exit(2);
        }
    if (cread==-1){
        perror("read");
        exit(3);
    }
    if (close(fs)==-1){
        perror("fs");
        exit(4);
    }
    exit(0);
    }
}

int main(int argc, char *argv[]){
    if (argc!=2){
        fprintf(stderr,"Usage %s fichier\n",argv[0]);
        exit(1);
    }
    ecrire_dans_stdout(argv[1]);
    return 0;
}

