#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>

#define MAX_PATH_LENGTH 1024
#define MAGIC_NUMBER_LENGTH 5
#define N 4 // Nombre de fils traiterFichier

typedef struct {
    char nom_fichier[MAX_PATH_LENGTH];
    int num_inode;
    off_t taille;
    char magic_number[MAGIC_NUMBER_LENGTH];
} ReponseFichier;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s magic_number repertoire\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *magic_number = argv[1];
    const char *repertoire = argv[2];

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
    close(tube_reponse[1]);

    return 0;
}
