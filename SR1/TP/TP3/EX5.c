#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>

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
    struct passwd* pwd = getpwuid(Infos->st_uid);
    printf("%-20s%10ld octets   %s  %s\n", type,Infos->st_size,pwd->pw_name, ctime(&Infos->st_mtime) );
}

int main(int argc, char* argv[]){
    if (argc<2){
        fprintf(stderr,"Usage %s fichiers",argv[0]);
        exit(1);
    }
    struct stat Infos;
    if (lstat(argv[1],&Infos)==-1){
        perror("lstat");
        exit(2);
    }
    printf("%-10s",argv[1]);
    Affiche_inode(&Infos);
    if (argc==2 && S_ISDIR(Infos.st_mode)){
        DIR* current = opendir(argv[1]);
        struct dirent* element;
        while((element= readdir(current))!=NULL){
            if(lstat(element->d_name,&Infos)==-1){
                perror(element->d_name);
            } else if ((strcmp(element->d_name,"."))&&(strcmp(element->d_name,".."))){
                printf("%s/%-10s",argv[1],element->d_name);
                Affiche_inode(&Infos);
            }
        }
    }
    for (int i =2; i<argc;i++){
        struct stat Infos;
        if (lstat(argv[i],&Infos)==-1){
            perror("lstat");
            exit(2);
        }
        printf("%-10s",argv[i]);
        Affiche_inode(&Infos);
    }
    return 0;
}