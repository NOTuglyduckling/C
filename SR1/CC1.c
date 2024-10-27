#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h> // Pour open()

#define BUFFER_SIZE 1024

// Fonction pour compter les occurrences d'un caractère dans un fichier
int NbOccurences(char Fichier[], char CarRech) {
    int f;
    char buffer[BUFFER_SIZE];
    int nb = 0;
    ssize_t bytesRead; // Nombre de caractères lus à chaque appel de read()

    // Ouvrir le fichier en lecture seule
    if ((f = open(Fichier, O_RDONLY)) == -1) {
        perror("open");
        exit(1);
    }

    // Lire le fichier par blocs de 1024 octets
    while ((bytesRead = read(f, &buffer, BUFFER_SIZE)) > 0) {
        // Parcourir chaque octet du bloc et compter les occurrences du caractère recherché
        for (ssize_t i = 0; i < bytesRead; i++) {
            if (buffer[i] == CarRech) {
                nb++;
            }
        }
    }

    // Gestion des erreurs de lecture
    if (bytesRead == -1) {
        perror("read");
        close(f);
        exit(2);
    }

    close(f);
    return nb;
}

int main(int argc, char* argv[]) {
    // Vérifier les arguments
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <caractère> <fichier1> <fichier2> ...\n", argv[0]);
        exit(1);
    }

    char CarRech = argv[1][0];  // Le caractère à rechercher (premier caractère de argv[1])

    for (int i = 2; i < argc; i++) {
        int com[2];  // Créer un pipe pour chaque itération/chaque fichier

        // Créer un pipe
        if (pipe(com) == -1) {
            perror("pipe");
            exit(2);
        }

        pid_t pid = fork();

        if (pid == -1) {  // Erreur lors du fork
            perror("fork");
            exit(3);
        }

        if (pid == 0) {  // Processus fils
            close(com[0]);  // Fermer la lecture du pipe dans le fils

            int nb = NbOccurences(argv[i], CarRech);  // Compter les occurrences du caractère
            printf("[fils %d] Je m'occupe de %s\n", i, argv[i]);

            // Envoyer le résultat au parent
            if (write(com[1], &nb, sizeof(int)) == -1) {
                perror("write");
                exit(4);
            }

            close(com[1]);  // Fermer l'écriture du pipe dans le fils
            exit(0);  // Terminer le processus fils
        } else {  // Processus père
            close(com[1]);  // Fermer l'écriture du pipe dans le père

            int nb;
            wait(NULL);  // Attendre que le fils termine

            // Lire le résultat du pipe
            if (read(com[0], &nb, sizeof(int)) == -1) {
                perror("read");
                exit(5);
            }

            // Afficher le résultat
            printf("[Père] %s contient %d fois le caractère '%c'\n", argv[i], nb, CarRech);

            close(com[0]);  // Fermer la lecture du pipe dans le père
        }
    }

    return 0;
}
