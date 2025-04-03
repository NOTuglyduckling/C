#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s NF NM\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int NF = atoi(argv[1]);
    int NM = atoi(argv[2]);
    pid_t pid;
    
    for (int i = 0; i < NF; i++) {
        pid = fork();
        if (pid < 0) {
            perror("Erreur fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { 
            for (int j = 0; j < NM; j++) {
                printf("Fils %d (PID=%d) : Message %d\n", i, getpid(), j + 1);
            }
            exit(i); 
        }
    }

    int status;
    pid_t child_pid;
    while ((child_pid = wait(&status)) > 0) {
        if (WIFEXITED(status)) {
            printf("Père : Fils avec PID %d terminé avec code %d\n", child_pid, WEXITSTATUS(status));
        }
    }

    return 0;
}
