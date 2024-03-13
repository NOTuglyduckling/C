#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>

#define MAX_PATH_LENGTH 1024
#define MAX_MAGIC_NUMBER_LENGTH 1024
#define MAGIC_NUMBER_LENGTH 5
#define N 4 // Nombre de fils traiterFichier

typedef struct {
    char nom_fichier[MAX_PATH_LENGTH];
    int num_inode;
    off_t taille;
    char magic_number[MAGIC_NUMBER_LENGTH];
} ReponseFichier;

void convertirMagicNumber(const char *magic_number_str, char *magic_number_hex) {
    char *token = strtok(magic_number_str, " ");
    while (token != NULL) {
        // Convertir chaque token en valeur hexadécimale et l'ajouter à la chaîne résultante
        int value;
        sscanf(token, "%x", &value);
        sprintf(magic_number_hex, "%02X", value);
        magic_number_hex += 2;
        token = strtok(NULL, " ");
    }
}

void executerCommande(const char *commande, const char *fichier) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        execlp(commande, commande, fichier, NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4 && argc != 5) {
        fprintf(stderr, "Usage: %s -v cmd magic_number repertoire\n", argv[0]);
        fprintf(stderr, "       %s -b magic_number_hex repertoire\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *commande;
    char magic_number_hex[MAX_MAGIC_NUMBER_LENGTH];

    if (strcmp(argv[1], "-v") == 0) {
        commande = argv[2];
        const char *magic_number_str = argv[3];
        convertirMagicNumber(magic_number_str, magic_number_hex);
    } else if (strcmp(argv[1], "-b") == 0) { // Option -b
        commande = NULL;
        const char *magic_number_str = argv[2];
        strncpy(magic_number_hex, magic_number_str, MAX_MAGIC_NUMBER_LENGTH - 1);
    } else {
        fprintf(stderr, "Option invalide.\n");
        exit(EXIT_FAILURE);
    }

    const char *repertoire = argv[argc - 1];

    // Création des tubes
    int tube_traiter[2];
    int tube_reponse[2];
    if (pipe(tube_traiter) == -1 || pipe(tube_reponse) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Création des processus fils traiterFichier
    for (int i = 0; i < N; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) { // Code des fils traiterFichier
            close(tube_traiter[1]); // Fermeture du descripteur d'écriture du tube dans les fils
            close(tube_reponse[0]); // Fermeture du descripteur de lecture du tube dans les fils
            // Code de traitement des fichiers
            // ...
            exit(EXIT_SUCCESS);
        }
    }

    // Création du processus fils traiterReponse
    pid_t pid_reponse = fork();
    if (pid_reponse == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid_reponse == 0) { // Code du fils traiterReponse
        close(tube_reponse[1]); // Fermeture du descripteur d'écriture du tube dans le fils
        close(tube_traiter[0]); // Fermeture du descripteur de lecture du tube dans le fils
        // Code de traitement des réponses
        // ...
        exit(EXIT_SUCCESS);
    }

    // Processus père : parcourt l'arborescence et communique les noms des fichiers aux fils traiterFichier
    // ...
    
    // Attente de la fin des fils traiterFichier
    for (int i = 0; i < N; i++) {
        wait(NULL);
    }

    // Attente de la fin du fils traiterReponse
    wait(NULL);

    // Fermeture des tubes
    close(tube_traiter[0]);
    close(tube_traiter[1]);
    close(tube_reponse[0]);
    close(tube_reponse[1]);

    return 0;
}
