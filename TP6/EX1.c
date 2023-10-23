#include <stdio.h>
#include <stdlib.h>

typedef unsigned char** tMatrice;
tMatrice MatAllouer(int* pNbLig, int* pNbCol);
tMatrice MatLire(int* pNbLig, int* pNbCol);
void MatAfficher(tMatrice Mat, int NbLIg, int NbCol);
tMatrice MatCopier(tMatrice Mat, int NbLig, int NbCol);
void MatLiberer(tMatrice* pMat);

int main(void) {
    int NbLig, NbCol;

    tMatrice mat = MatLire(&NbLig, &NbCol);

    if (mat == NULL) {
        printf("Error allocating or reading the matrix.\n");
        return NULL;
    } else {
        printf("Matrix with %d rows and %d columns has been successfully created.\n", NbLig, NbCol);

        printf("Original Matrix:\n");
        MatAfficher(mat, NbLig, NbCol);


        tMatrice copy = MatCopier(mat, NbLig, NbCol);
        if (copy != NULL) {
            printf("Copy of the Matrix:\n");
            MatAfficher(copy, NbLig, NbCol);

            MatLiberer(&copy);
        } else {
            printf("Failed to create a copy of the matrix.\n");
            return NULL;
        }
        MatLiberer(&mat);
    }
    return 0;
}


tMatrice MatAllouer(int* pNbLig, int* pNbCol){
    tMatrice matrice = malloc(*pNbLig * sizeof(unsigned char*)); //tMatrice= unsigned char**
    for (int i=0 ; i<*pNbLig ; i++) {
        matrice[i] = malloc(*pNbCol * sizeof(unsigned char));
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

void MatAfficher(tMatrice Mat, int NbLIg, int NbCol){
    for (int i = 0 ; i < NbLIg ; i++){
        for (int j = 0 ; j < NbCol ; j++){
            printf("%3hhu ",Mat[i][j]);
        }
        printf("\n");
    }
}

tMatrice MatCopier(tMatrice Mat, int NbLig, int NbCol){
    tMatrice Copie = MatAllouer(&NbLig,&NbCol);
    if (Copie==NULL){
        printf("Erreur d'allocation memoire ! \n");
        return NULL;
    }
    for (int i=0 ; i<NbLig ; i++ ){
        for (int j=0 ; j<NbCol ; j++ ){
            Copie[i][j] = Mat[i][j];
        }
    }
    return Copie;
}

void MatLiberer(tMatrice *pMat) {
    if (*pMat != NULL) {
        // Free individual rows
        for (int i = 0; i < *pMat[0]; i++) {
            free((*pMat)[i]);
        }
        free(*pMat);
        *pMat = NULL;
    }
}
