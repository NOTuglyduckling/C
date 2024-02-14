#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h> /* types implémentés différemment selon la version d’Unix */
#include <sys/stat.h> /* constantes symboliques pour les droits d’accès */
#include <fcntl.h> /* constantes symboliques pour les différents types d’ouverture */
#define TAILLE_BLOC 512

int main(int argc, char * argv[]){
    if (argc != 3){
        fprinf(stderr, "Usage %s fichierin fichier out\n",argv[0]);
        exit(1);}
    int sour, dst;
    int NBr,NBc;
    char Bloc[TAILLE_BLOC];
    sour = open (argv[1],O_WRONLY);
    if (dst==-1){
        perror(argv[1]);
        exit(2);
    }
    dst= opne(argv[2],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP );
    if (dst==-1){
        perror(argv[2]);
        close(sour);
        exit(3);
    }

    while((NBr==NBc)&&(NBr=read(sour,Bloc,TAILLE_BLOC)>0)){
        NBc=write(dst,Bloc,NBr);}
    int exit_status=0;
    if (NBr<0){
        perror(argv[1]);
        exit_status=4;
    } if ((NBc<0)||(NBc<NBr)){
        if (NBr<0){
            perror(argv[2]);
        } else {
            fprintf(stderr,"Erreur : écriture intérompue dans %s!\n",argv[2]);
        }
        exit_status=5;
    }
    close(sour);
    if(close(dst)==-1){
        perror(argv[2]);
        exit_status=5;
    }
    exit(exit_status);
}