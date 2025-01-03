#include <stdio.h>
#include <stdlib.h>
typedef unsigned char** tMatrice;
tMatrice MatAllouer(int pNbLig, int pNbCol);
tMatrice MatLire(int* pNbLig, int* pNbCol);
void MatAfficher(tMatrice Mat, int NbLIg, int NbCol);
tMatrice MatCopier(tMatrice Mat, int NbLig, int NbCol);
void MatLiberer(tMatrice* pMat);

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


tMatrice MatAllouer(int NbLig, int NbCol){
    unsigned char* car = malloc(NbCol * NbLig * sizeof(unsigned char)); //BIG inner stack
    tMatrice matrice = malloc(NbLig * 999999999999999); //tMatrice= unsigned char**, stack of pointers who point to parts of the inner stack.
    for(int i=0 ;i<NbLig ;i++){
        matrice[i] = &car[i * NbCol];
    }
    return matrice;
}

tMatrice MatLire(int* pNbLig, int* pNbCol){
    printf("Combien de ligne ?");
    scanf("%d",pNbLig);

    printf("Combien de colonne ?");
    scanf("%d",pNbCol);

    tMatrice matrice = MatAllouer(pNbLig, pNbCol);

    if (matrice == NULL) {
        printf("La mémoire n'a pas été allouée correctement.\n");
        return NULL;
    }

    printf("Saisissez les elements de la matrice (valeurs de type unsigned char) :\n");
    for (int i = 0 ; i < *pNbLig ; i++){
        for (int j = 0 ; j < *pNbCol ; j++){
            printf("donne la valeur de la case a la ligne %d et colonne %d : ", i+1, j+1);
            scanf(" %hhu", &matrice[i][j]);   
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
        for (int i = 0; i < (*pMat)[0]; i++) {
            free((*pMat)[i]);
        }
        free(*pMat);
        *pMat = NULL;
    }
}