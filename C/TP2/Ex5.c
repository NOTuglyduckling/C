#include <stdio.h>

int main() {
    int N;
    do{
        printf("Insira um valor: ");
        scanf("%d", &N);
    } while ( N <= -1  || N>18);
    int ligne, colonne;

    printf("x |");

    // Afficher l'en-tête des colonnes
    for (colonne = 0; colonne <= N; colonne++) {
        printf("%3d ", colonne);
    }

    printf("\n-----------------------------------------------\n");

    // Afficher le corps de la table de multiplication
    for (ligne = 0; ligne <= N; ligne++) {
        printf("%d |", ligne);

        for (colonne = 0; colonne <= N; colonne++) {
            printf("%3d ", ligne * colonne);
        }

        printf("\n");
    }

    return 0;
}
