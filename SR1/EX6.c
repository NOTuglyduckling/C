#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
    char* fichier;
    int fd = open(fichier,O_RDWR);
    off_t position;
    position=lseek(fd,0,SEEK_CUR);
    lseek(fd,position,SEEK_SET);
    off_t taille;
    taille= lseek(fd,0,SEEK_END);
}

int Nieme(int fd, int n){
    lseek(fd,sizeof(int)*n,SEEK_SET);
    int entier;
    read(fd,&entier,sizeof(int));
    return entier;
}