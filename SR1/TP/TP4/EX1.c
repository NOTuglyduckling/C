#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

int main(){
    int tube[2];
    if (pipe(tube)==-1){
        perror("tube");
        exit(2);
    }
    pid_t pid=fork();
    if (pid==-1){
        perror("fork");
        exit(3);
    }
    if (pid==0){
        close(tube[1]);
        int n;
        while (read(tube[0], &n, sizeof(int))>0)
            printf("[Fils] %d\n",n);
        close(tube[0]);
        exit(1);
    } else {
        close(tube[0]);
        int N=4;
        for (int i =0; i<=N; i++){
            write(tube[1],&i,sizeof(int));
            sleep(1);
        }
        close(tube[1]);
        int status;
        waitpid(pid,&status,0);
        printf("[Pere] : Mon fils %d est termine avec le code %d.",pid,WEXITSTATUS(status));
    }
    return 0;
}