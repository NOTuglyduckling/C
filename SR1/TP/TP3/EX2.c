#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

void Affiche_inode (struct stat * Infos){
    char * type;
    if (S_ISREG(Infos->st_mode)){
        type = "fichier ordinaire";
    } else if (S_ISDIR(Infos->st_mode)){
        type = "repertoire";
    } else if (S_ISLNK(Infos->st_mode)){
        type = "lien";
    } else if (S_ISBLK(Infos->st_mode)){
        type = "perepherique bloc";
    } else if (S_ISCHR(Infos->st_mode)){
        type = "perepherique charactère";
    } else if (S_ISFIFO(Infos->st_mode)){
        type = "Pipe";
    } else if (S_ISSOCK(Infos->st_mode)){
        type = "socket";
    } else {type="inconnu";}
    printf("%-20s%10ld octets  %s\n", type,Infos->st_size, ctime(&Infos->st_mtime) );
}

int main(int argc, char* argv[]){
    if (argc<2){
        fprintf(stderr,"Usage %s fichiers",argv[0]);
        exit(1);
    }
    
    for (int i =1; i<argc;i++){
        struct stat Infos;
        if (lstat(argv[i],&Infos)==-1){
            perror("lstat");
            exit(1);
        }
        printf("%-10s",argv[i]);
        Affiche_inode(&Infos);
    }
    return 0;
}