#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s NF NB NS\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int NF = atoi(argv[1]);
    int NB = atoi(argv[2]);
    int NS = atoi(argv[3]);

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("Erreur pipe");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < NF; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Erreur fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { 
            close(pipefd[1]);
            int chocolates, boxes = 0;
            while (read(pipefd[0], &chocolates, sizeof(int)) > 0) {
                int size = (i + 1) * 6;
                while (chocolates >= size) {
                    chocolates -= size;
                    boxes++;
                    printf("Fils %d : Emballé une boîte de %d chocolats. Total boîtes : %d\n", i, size, boxes);
                }
            }
            close(pipefd[0]);
            printf("Fils %d terminé. Boîtes produites : %d\n", i, boxes);
            exit(0);
        }
    }

    close(pipefd[0]);
    for (int j = 0; j < NB; j++) {
        int chocolates = 30;  
        printf("Père : Produit %d chocolats\n", chocolates);
        write(pipefd[1], &chocolates, sizeof(int));
        sleep(NS);
    }
    close(pipefd[1]);

    while (wait(NULL) > 0);
    return 0;
}
