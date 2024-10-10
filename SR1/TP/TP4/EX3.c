#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <math.h>


int est_premier (unsigned long long int n)
{
  if ((n>1) && (n==2 || n==3 || ((n%2)&&(n%3))))
  {
    unsigned long long int i,y;

    y=sqrt(n)+1;
    i=5;
    while(i<y)
    {
      if (n%i == 0)
        return 0;
      i+=2;
      if (n%i == 0)
        return 0;
      i+=4;
    }
    return 1;
  }
  return 0;
}

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
    for (int i = 0; i < fils; i++){
        pid_t pid = fork();
        if (pid == -1){
            perror("fork");
            exit(3);
        }
        if (pid == 0){
            close(tube[1]);
            unsigned long long int entier;
            while (read(tube[0], &entier, sizeof(unsigned long long int)) > 0){
                if (est_premier(entier)) {
                    printf("[Fils %d] %llu est premier\n", i ,entier);
                    fflush(stdout);
                    cpremiers++;
                    sleep(1);
                } else {
                    printf("[Fils %d] %llu\n", i,entier);
                }
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
            close(tube[1]);
            exit(4);
        }
    }
    close(tube[1]);
    int status;
    for (int i = 0; i < fils; i++){
        wait(&status);
        cpremiers += WEXITSTATUS(status);
    }
    printf("Il y a %d nombres premiers.\n", cpremiers);
    return 0;
}