#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h>
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

int main(int argc, char* argv[]){
    int in,out;
    if(argc!>2){
        fprintf(stderr,"Usage : %s fichierIn(s) [\">\" fichierOut]\n",argv[0]);
        exit(1);}
    int stop = argc;
    for (int i=1; i<argc;i++){
        if (argv[i]==">"){
            stop=i;
        }
    } if (stop!=argc){
        out= open(argv[argc],O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IROTH|S_IWUSR);
            if (out==-1){
                perror(argv[3]);
                close(in);
                exit(3);}
        if (dup2(out,STDOUT_FILENO)==-1){
            perror("STDOUT");
            close(out);
            exit(4);}
    }
    for (int i = 1; i<stop;i++){
        printf("Affichage fichier %s",argv[i]),
        in= open(argv[i],O_RDONLY);
        ecrire_dans_stdout(in);
    }
    close(in);
    close(out);
    exit(0);
}