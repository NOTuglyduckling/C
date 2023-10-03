#include <stdio.h>
#include <math.h>

#define EPSILON 1e-10 // Valeur de tolérance ε

// Fonction pour vérifier si deux nombres réels sont égaux avec une tolérance ε
int sontEgaux(double x, double y) {
    return fabs(x - y) < EPSILON;
}

// Fonction pour vérifier si une matrice est symétrique
int estSymetrique(double matrice[][100], int lignes, int colonnes) {
    if (lignes != colonnes) {
        return 0; // La matrice doit être carrée pour être symétrique
    }

    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < colonnes; j++) {
            // Comparer les éléments de la matrice avec tolérance ε
            if (!sontEgaux(matrice[i][j], matrice[j][i])) {
                return 0; // Les éléments ne correspondent pas symétriquement
            }
        }
    }

    return 1; // La matrice est symétrique
}

int main() {
    int lignes, colonnes;
    double matrice[100][100]; // Utilisation d'une matrice de taille maximale de 100x100

    printf("Entrez le nombre de lignes de la matrice : ");
    scanf("%d", &lignes);

    printf("Entrez le nombre de colonnes de la matrice : ");
    scanf("%d", &colonnes);

    if (lignes <= 0 || colonnes <= 0 || lignes > 100 || colonnes > 100) {
        printf("Taille de matrice invalide.\n");
        return 1;
    }

    printf("Entrez les éléments de la matrice :\n");
    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < colonnes; j++) {
            scanf("%lf", &matrice[i][j]);
        }
    }

    if (estSymetrique(matrice, lignes, colonnes)) {
        printf("La matrice est symétrique.\n");
    } else {
        printf("La matrice n'est pas symétrique.\n");
    }

    return 0;
}
