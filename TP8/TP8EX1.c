#include "matrice.h"

int main(void) {
    int NbLig, NbCol;

    tMatrice mat = MatLire(&NbLig, &NbCol);

    printf("Matrix with %d rows and %d columns has been successfully created.\n", NbLig, NbCol);
    printf("Original Matrix:\n");
    MatAfficher(mat, NbLig, NbCol);

    tMatrice copy = MatCopier(mat, NbLig, NbCol);

    printf("Copy of the Matrix:\n");
    MatAfficher(copy, NbLig, NbCol);

    MatLiberer(&copy); 
    MatLiberer(&mat);
    return 0;
}