#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void main(){
    int tubeA[2];
    int tubeC[2];
    char car;
    if(pipe(tube)==-1){
        perror("prbl creation tube");
        exit(1);
    }
    pid_t pid =fork();
    switch(pid){
        case -1:{
            perror("prblm creation tube");
            exit(2);
        }
        case 0:{
            close(tube[1]);
            while(read(tube[0],&car, sizeof(car)))
                write(STDOUT_FILENO,&car,sizeof(car));
            close(tube[0]);
            exit(0);
        }
        default : {
            close(tube[0]);
            while(read(STDIN_FILENO,&car,sizeof(car)))
                write(tube[1],&car,sizeof(car));
            close(tube[1]);
            exit(0);
        }
    }
}