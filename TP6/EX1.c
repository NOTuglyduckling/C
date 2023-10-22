#include <stdio.h>
#include <stdlib.h>

typedef unsigned char** tMatrice;
tMatrice MatAllouer(int* pNbLig, int* pNbCol);
tMatrice MatLire(int* pNbLig, int* pNbCol);

int main(void){
    int NbLig, NbCol;
    tMatrice mat = MatLire(&NbLig, &NbCol);

    if (mat == NULL) {
        printf("Error allocating or reading the matrix.\n");
        return NULL;
    } else {
        printf("Matrix with %d rows and %d columns has been successfully created.\n", NbLig, NbCol);

        // Here, you can use the 'mat' matrix as needed.

        // Don't forget to free the memory when you're done.
        for (int i = 0; i < NbLig; i++) {
            free(mat[i]);
        }
        free(mat);
    }
    return 0;
}

tMatrice MatAllouer(int* pNbLig, int* pNbCol){
    tMatrice matrice = (tMatrice)malloc(*pNbLig * sizeof(unsigned char*)); //tMatrice= unsigned char**
    if (matrice == NULL) {
        printf("Erreur d'allocation de la mémoire\n");
        return NULL;
    }
    for (int i=0 ; i<*pNbLig ; i++) {
        matrice[i] = (unsigned char*)malloc(*pNbCol * sizeof(unsigned char));
        if (matrice[i]==NULL) {
            for (int j = 0; j < i; j++) {
                free(matrice[j]);
            }
            free(matrice);
            return NULL;
        }
    }
    return matrice;
}

tMatrice MatLire(int* pNbLig, int* pNbCol){
    printf("Combien de ligne ?");
    scanf("%d",pNbLig);

    printf("Combien de colonne ?");
    scanf("%d",pNbCol);

    tMatrice matrice = MatAllouer(&pNbLig, &pNbCol);

    if (matrice == NULL) {
        printf("La mémoire n'a pas été allouée correctement.\n");
        return NULL;
    }

    printf("Saisissez les éléments de la matrice (valeurs de type unsigned char) :\n");
    for (int i = 0 ; i < *pNbLig ; i++){
        for (int j = 0 ; j < *pNbCol ; j++){
            if (scanf(" %hhu", &matrice[i][j]) != 1) {
                // If input is not a valid unsigned char, free the allocated memory and return NULL
                for (int k = 0; k < i; k++) {
                    free(matrice[k]);
                }
                free(matrice);
                return NULL; 
            }
        }
    }
    return matrice;
}

