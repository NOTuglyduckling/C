#include <stdio.h>

int main() {
    // Déclaration et initialisation d'une matrice 3x3
    int matrice[3][3] = {
        {1, 2, 3},
        {2, 5, 6},
        {3, 6, 9}
    };

    // Affichage de la matrice
    printf("Matrice 3x3 :\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", matrice[i][j]);
        }
        printf("\n");
    }
    for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
            if (matrice[i][j]!=matrice[j][i]){
                printf("pas sym");
                return 0;
            }   
        }
    }
    printf("sym");
    return 1;
}
