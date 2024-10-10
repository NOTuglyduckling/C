#include <stdio.h>

int main() {
    int n;

    printf("Entrez le nombre de lignes pour le triangle : ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("Le nombre de lignes doit être supérieur à zéro.\n");
        return 1;
    }

    for (int i = 1; i <= n; i++) {
        // Afficher les espaces à gauche du triangle
        for (int j = 1; j <= n - i; j++) {
            printf(" ");
        }

        // Afficher les étoiles
        for (int k = 1; k <= 2 * i - 1; k++) {
            printf("*");
        }

        printf("\n");
    }

    return 0;
}
