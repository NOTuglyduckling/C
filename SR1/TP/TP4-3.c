#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include "premier.h"

int main(int argc, char *argv[]){
    if (argc < 2){
        fprintf(stderr, "Usage: %s n1 n2 ...\n", argv[0]);
        exit(1);
    }
    int fils = 4;
    int cpremiers = 0;
    int tube[2];
    if (pipe(tube) == -1){
        perror("pipe");
        exit(2);
    }
    for (int i = 1; i < N; i++){
        unsigned long long int nombre = strtoull(argv[i], NULL, 10);
        pid_t pid = fork();
        if (pid == -1){
            perror("fork");
            exit(3);
        }
        if (pid == 0){
            close(tube[1]);
            int entier;
            while (read(tube[0], &entier, sizeof(int)) > 0){
                if (est_premier(entier)) {
                    printf("[Fils %d] %llu est premier\n", i - 1, nombre);
                    fflush(stdout);
                    cpremiers++;}
            }
            close(tube[0]);
            exit(cpremiers);
        }
    }
    close(tube[0]);
    for (int i = 1; i < argc; i++){
        unsigned long long int nombre = strtoull(argv[i], NULL, 10);
        if (write(tube[1], &nombre, sizeof(unsigned long long int)) == -1) {
            perror("write");
            exit(4);
        }
    }
    close(tube[1]);
    int status;
    for (int i = 0; i < fils; i++){
        wait(&status);
        if (WIFEXITED(status)){
            cpremiers += WEXITSTATUS(status);
        }
    }
    printf("Il y a %d nombres premiers.\n", cpremiers);
    return 0;
}
