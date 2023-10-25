#include <stdio.h>
#include <stdlib.h>

typedef unsigned char** tMatrice;
tMatrice MatAllouer(int pNbLig, int pNbCol);
void AffTriangle(tMatrice Mat, int NbLIg);
void MatLiberer(tMatrice* pMat);
tMatrice Triangle(int n);

int main(void) {
    int n;
    printf("donne ton N");
    scanf("%d", &n);
    tMatrice mat = Triangle(n);

    printf("Matrix with %d rows and %d columns has been successfully created.\n", n, n);
    printf("Original Matrix:\n");
    AffTriangle(mat, n);

    MatLiberer(&mat);
    return 0;
}

tMatrice MatAllouer(int pNbLig, int pNbCol){
    unsigned char* car = malloc(pNbCol * pNbLig * sizeof(unsigned char)); //BIG inner stack
    tMatrice matrice = malloc(pNbLig * sizeof(unsigned char*)); //tMatrice= unsigned char**, stack of pointers who point to parts of the inner stack.
    for(int i=0 ;i<pNbLig ;i++){
        matrice[i] = &car[i * pNbCol];
    }
    return matrice;
}

void AffTriangle(tMatrice Mat, int NbLIg){
    for (int i = 0 ; i < NbLIg ; i++){
        int j=0;
        while (j <= i){
            printf("%3u ",Mat[i][j++]);
        }
        printf("\n");
    }
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

tMatrice Triangle(int n){
    tMatrice A = MatAllouer(n,n);
    A[0][0]=A[1][0]=A[1][1]=1;
    for (int i = 2; i < n ; i++){
        int j=1;
        while (j < i){
            A[i][j]=A[i-1][j-1]+A[i-1][j];
            j++;
        }
        A[i][j]=A[i][0]=1;
    }     
    return A;
}