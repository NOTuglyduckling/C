#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
    int in,out;
    if(argc!=4){
        fprintf(stderr,"Usage : %s Command fichierIn fichierOut\n",argv[0]);
        exit(1);}
    in= open(argv[2],O_RDONLY);
    if (in==-1){
        perror(argv[2]);
        exit(2);}
    out= open(argv[3],O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IROTH|S_IWUSR);
    if (out==-1){
        perror(argv[3]);
        close(in);
        exit(3);}
    if (dup2(in,STDIN_FILENO)==-1){
        perror("STDIN");
        close(in);
        close(out);
        exit(4);}
    if (dup2(out,STDOUT_FILENO)==-1){
        perror("STDOUT");
        close(in);
        close(out);
        exit(4);}
    pid_t pid = fork();
    if (pid==-1){
        perror("fork");
        close(in);
        close(out);
        exit(5);}
    else if (pid==0){
        execlp(argv[1], argv[1],NULL);
        perror(argv[1]);
        close(in);
        close(out);
        exit(6);}
    wait(NULL);
    close(in);
    close(out);
    exit(0);
}