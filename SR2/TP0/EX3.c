#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define MONTANT_MAX 1000.0

typedef struct {
    char ref[3];
    int quantity;
    double price;
} Devis;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s NF\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int NF = atoi(argv[1]);
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
            close(pipefd[0]);
            srand(getpid());
            Devis devis = {"AB", 50, (double)(rand() % (int)MONTANT_MAX)};
            write(pipefd[1], &devis, sizeof(Devis));
            close(pipefd[1]);
            exit(0);
        }
    }

    close(pipefd[1]);
    Devis meilleur_devis, devis;
    int first = 1;
    while (read(pipefd[0], &devis, sizeof(Devis)) > 0) {
        printf("Client : Offre reçue -> Produit %s, Quantité %d, Prix %.2f\n", devis.ref, devis.quantity, devis.price);
        if (first || devis.price < meilleur_devis.price) {
            meilleur_devis = devis;
            first = 0;
        }
    }
    close(pipefd[0]);

    printf("Client : Offre retenue -> Produit %s, Quantité %d, Prix %.2f\n", meilleur_devis.ref, meilleur_devis.quantity, meilleur_devis.price);

    while (wait(NULL) > 0);
    return 0;
}
