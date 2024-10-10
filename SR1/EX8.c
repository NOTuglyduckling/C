#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

long TailleRepCour(){
    long oct=0;
    struct stat infos;
    DIR* current = opendir(".");
    struct dirent* element;
    if (current == NULL){
        perror(".");
        return -1;
    }
    while ((element = readdir(current)) != NULL){
        if (lstat(element->d_name, &infos) == -1){
            perror(element->d_name);
        } else if (S_ISREG(infos.st_mode)){ 
            oct += infos.st_size;
        }
    }
    closedir(current);
    return oct;
}

void namesrep(char nomRep[], int *num,int * size){
    struct stat infos;
    DIR* current = opendir(nomRep);
    if (current == NULL){
        perror(nomRep);
        return;
    }
    struct dirent* element;
    char designation[512];
    
    while ((element = readdir(current)) != NULL){
        if (strcmp(element->d_name, ".") && strcmp(element->d_name, "..")){
            strncpy(designation, nomRep, 511);
            strcat(designation, "/");
            strncat(designation, element->d_name, 511);
            if (lstat(designation, &infos) == -1)
                perror(designation);
            *size+=infos.st_size;
            (*num)++; 
            if (S_ISREG(infos.st_mode)){
                printf("%s\n", designation); 
            } else if(S_ISDIR(infos.st_mode)){    
                namesrep(designation, num,size);}
        }
    }
    closedir(current);
}

int main(int argc, char* argv[]){
    if(argc != 2){
        fprintf(stderr, "Usage %s directory\n", argv[0]);
        exit(1);
    }
    int size=0;
    struct stat inode;
    if(lstat(argv[1], &inode) == -1){
        perror(argv[1]);
        exit(2);
    }
    printf("taille rep courant : %ld octets.\n", TailleRepCour());

    int num = 0;  // Initialisation de num comme entier
    namesrep(argv[1], &num,&size);  // Passage de l'adresse de num à la fonction
    printf("nombre de trucs : %d\n", num);  // Affichage correct du compteur

    printf("%s ", argv[1]);
    printf("%d octets, ", size);
    printf("dernière modification %s\n", ctime(&inode.st_mtime));

    return 0;
}

