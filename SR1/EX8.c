#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

int main(int argc, char* argv[]){
    if(argc!=2){
        fprintf(stderr,"Usage %s file \n",argv[0]);
        exit(1);
    }
    struct stat inode;
    if(lstat(argv[1],&inode)==1){
        perror(argv[1]);
        exit(2);
    }
    printf("%s ",argv[1]);
    printf("%d octets, ", inode.st_size);
    printf("dernière modification %s\n",ctime(&inode.st_mtime));
    exit(0);
}

long TailleRepCour(){
    long oct=0;
    struct stat infos;
    DIR* current = opendir(".");
    struct dirent* element;
    if (current==NULL){
        perror(".");
        return -1;
    }
    while ((element = readdir(current))!=NULL){
        if (lstat(element->d_name,&infos)==-1){
            perror(element->d_name);
        }else if (S_ISREG(infos.st_mode)){ 
            oct+=infos.st_size;
        }
    }
    closedir(current);
    return oct;
}

void namesrep(char nomRep[]){
    struct stat infos;
    DIR* current = opendir(nomRep);
    struct dirent* element;
    char designation[512];
    if (current==NULL){
        perror(nomRep);
        return;
    }
    while ((element = readdir(current))!=NULL){
        if (strcmp(element->d_name,".")&&strcmp(element->d_name,"..")){
            strncpy(designation,nomRep,511);
            strncat(designation,"/",511);
            strncat(designation,element->d_name,511);
            if (lstat(designation,&infos)==-1)
                perror(designation);
            }else if (S_ISREG(infos.st_mode))
                printf("%s\n",designation);
            else if(S_ISDIR(infos.st_mode))
                namesrep(designation);
    }
    closedir(current);
    return;
}