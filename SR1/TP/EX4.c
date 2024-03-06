#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>

void parcourir_sous_arborescence(const char *repertoire, int *compteur) {
    DIR *dir;
    struct dirent *entree;
    struct stat infos;

    if ((dir = opendir(repertoire)) == NULL) {
        perror("opendir");
        exit(1);
    }

    while ((entree = readdir(dir)) != NULL) {
        char chemin[1024];
        snprintf(chemin, sizeof(chemin), "%s/%s", repertoire, entree->d_name);

        if (stat(chemin, &infos) == -1) {
            perror("stat");
            exit(2);
        }

        if (S_ISDIR(infos.st_mode)) {
            if (strcmp(entree->d_name, ".") != 0 && strcmp(entree->d_name, "..") != 0) {
                parcourir_sous_arborescence(chemin, compteur);
            }
        } else if (S_ISREG(infos.st_mode)) {
            if (infos.st_size == 0) {
                printf("%s\n", chemin);
                (*compteur)++;
            }
        }
    }

    closedir(dir);
}void Affiche_inode (struct stat * Infos){
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
        fprintf(stderr,"Usage %s fichivoid Affiche_inode (struct stat * Infos){
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

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <repertoire>\n", argv[0]);
        return 1;
    }

    int compteur = 0;
    parcourir_sous_arborescence(argv[1], &compteur);
    printf("%d fichier(s) vide(s) trouvé(s) !\n", compteur);

    return 0;
}