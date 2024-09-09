#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h> /* types implémentés différemment selon la version d’Unix */
#include <sys/stat.h> /* constantes symboliques pour les droits d’accès */
#include <fcntl.h> /* constantes symboliques pour les différents types d’ouverture */
#define TAILLE_BLOC 512

int main(int argc,char * argv[]){
    if (argc !=3){
        fprintf(stderr, "Usage %s fichierin fichierout\n",argv[0]);
        exit(1);
    }
    int file,filec;
    if ((file = open(argv[1],O_RDONLY))==-1){
        perror("Echec open");
        exit(2);
    }
    if ((filec = open(argv[2],O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR|S_IRGRP|S_IWUSR))==-1){
        perror("Echec open");
        exit(3);
    }
    char mem[TAILLE_BLOC];
    int NBlus,NBecr;
    while ((NBlus==NBecr)&&(NBlus = read(file,mem,TAILLE_BLOC))>0){
        NBecr = write(filec,mem,NBlus);
    }
    if (NBlus==-1){
        perror("Echec lecture");
        exit(4);
    }
    if ((NBecr<0)||(NBecr<NBlus)){
        if (NBlus<0){
            perror(argv[2]);
        } else {
            fprintf(stderr,"Erreur : écriture intérompue dans %s!\n",argv[2]);
        }
        exit(5);
    }

    close(file);
    if (close(filec)){
        perror("Erreur close");
        exit(6);
    }
    exit(0);
}