#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

void main(){
    int tubeA[2], tubeN[2];
    char car;
    if((pipe(tubeA)==-1)||(pipe(tubeB)==-1)){
        perror("prbl creation tube");
        exit(1);
    }
    pid_t pid = fork();
    switch(pid){
        case -1:{
            perror("prblm creation tube");
            exit(2);
        }
        case 0:{
            close(tubeA[1]);
            close(tubeN[1]);
            close(tubeN[0]);
            int tabA[26]; for (int i =0; i<26;i++) tabA[i]=0;
            while(read(tubeA[0],&car, sizeof(car))>0)
                tabA[tolower(car)-'a']++;
            close(tubeA[0]);
            for (int i=0;i<26;i++)
                printf("%c%-2d\n",(char)i+'a',tabA[i]);
            exit(0);
        }
    pid_t pid = fork();
    switch(pid){
        case -1:{
            perror("prblm creation tube");
            close(tubeA[1]);
            exit(2);
        }
        case 0:{
            close(tubeA[1]);
            close(tubeA[0]);
            close(tubeN[1]);
            int somme=0;
            while(read(tubeN[0],&car, sizeof(char))>0)
                somme+=car-'0';
            close(tubeN[0]);
            printf("Somme : %d\n",somme);
            exit(0);
        }
        
        close(tubeA[0]);
        close(tubeN[0]);
        while(read(STDIN_FILENO,&car,sizeof(car))>0){
            if (isalpha(car))
                write(tubeA[1],&car,sizeof(car));
            else if (isdigit(car))
                write(tubeN[1],&car,sizeof(car));} 
        close(tubeN[1]);
        close(tubeA[1]);
        wait(NULL);
        wait(NULL);
        exit(0);
    }
}