#include <stdio.h>

int main() {
    int ligne, colonne;

    printf("x |");

    // Afficher l'en-tête des colonnes
    for (colonne = 0; colonne <= 10; colonne++) {
        printf("%3d ", colonne);
    }

    printf("\n-----------------------------------------------\n");

    // Afficher le corps de la table de multiplication
    for (ligne = 0; ligne <= 10; ligne++) {
        printf("%d |", ligne);

        for (colonne = 0; colonne <= 10; colonne++) {
            printf("%3d ", ligne * colonne);
        }

        printf("\n");
    }

    return 0;
}
