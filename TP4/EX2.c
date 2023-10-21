#include <stdio.h>
const int T = 60;
const int T2 = 3600;
int demandeS();
void AfficherTab(int Tab[]);
void convert1(int* heures,int* minutes,int* secondes,int s);
void convert2(int tab[],int secondes);


int main (void){
    int heures;
    int minutes;
    int secondes;
    int tab[3];
    int s = demandeS();
    convert2(tab,s);
    AfficherTab(tab);
    convert1(&heures,&minutes,&secondes,s);
    printf("%d heures, %d minutes, %d secondes",heures,minutes,secondes);
}

int demandeS(){  
    int s;
    printf("Donnez le temps en secondes");
    scanf("%d", &s);
    return s;
}

void AfficherTab(int Tab[]){
    for (int i = 0; i < 3 ; ++i){
        printf("%d ",Tab[i]);
    }
    printf("\n");
}

void convert1(int* heures,int* minutes,int* secondes,int s){
    *heures = s / T2;
    *minutes = ( s / T )- (*heures * T);
    *secondes = s % T;
}

void convert2(int tab[], int secondes){
    tab[0] = secondes / T2;
    tab[1] = (secondes / T)-(tab[0] * T);
    tab[2] = secondes % T;
}

