#include <stdio.h>
#include <stdlib.h>

// Fonction pour lire ou écrire le Nième entier dans un fichier
void Nieme(int n, int entier, const char *fichier) {
    FILE *file = fopen(fichier, "rb+");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Se positionner au début du Nième entier dans le fichier
    if (fseek(file, (n - 1) * sizeof(int), SEEK_SET) != 0) {
        perror("Erreur lors du positionnement dans le fichier");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    if (entier == -1) { // Lecture du Nième entier
        int value;
        if (fread(&value, sizeof(int), 1, file) != 1) {
            perror("Erreur lors de la lecture du fichier");
            fclose(file);
            exit(EXIT_FAILURE);
        }
        printf("Nième entier : %d\n", value);
    } else { // Écriture du Nième entier
        if (fwrite(&entier, sizeof(int), 1, file) != 1) {
            perror("Erreur lors de l'écriture dans le fichier");
            fclose(file);
            exit(EXIT_FAILURE);
        }
        printf("Nième entier modifié avec succès\n");
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s n [entier] fichier\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Erreur : n doit être un entier positif\n");
        return EXIT_FAILURE;
    }

    const char *fichier = argv[3];
    int entier = -1; // Par défaut, on lit le Nième entier
    if (argc == 4) {
        entier = atoi(argv[2]);
    }

    Nieme(n, entier, fichier);

    return EXIT_SUCCESS;
}