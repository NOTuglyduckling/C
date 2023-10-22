#include <stdio.h>
#include <stdlib.h>

double* CreerTableau(int NbElts);
void SaisirTableau(double* Tab, int NbElts);
void AfficherTableau(double* Tab, int NbElts);
void LibererTableau(double *Tab);

int main(void){
    int NbElts = 0; 
    printf("donner la taille du tableau :");
    scanf("%d",&NbElts);
    double* Tab= CreerTableau(NbElts);
    if (Tab == NULL) {
        return 1; // Exit the program if memory allocation fails
    }
    SaisirTableau(Tab,NbElts);
    AfficherTableau(Tab,NbElts);
    LibererTableau(Tab);
    return 0;
}

double* CreerTableau(int NbElts){
    double* Tab = (double*)malloc(NbElts * sizeof(double));
    if(Tab == NULL) {  // si malloc a renvoye un pointeur null, allocation impossible
        printf("Erreur d'allocation memoire\n");
        return NULL;
    } else{
        return Tab;
    }
}

void SaisirTableau(double* Tab, int NbElts){
    for(int i = 0; i < NbElts; i++){
        printf("\ndonner le nombre %d: ",i+1);
        scanf("%lf", Tab + i);
    }
}

void AfficherTableau(double* Tab, int NbElts){
    for(int i = 0; i < NbElts; i++) {
        printf("%lf ", *(Tab+i));
    }
}

void LibererTableau(double *Tab){
    free(Tab);
}