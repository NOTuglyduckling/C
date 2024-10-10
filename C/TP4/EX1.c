#include <stdio.h>

void AfficherTab(int Tab[], int NbElt);
int Min(int Tab[], int NbElt);
int Max(int Tab[], int NbElt);
void MinMax(int Tab[], int NbElt, int *pMin, int *pMax);
int SupprimerVal(int Tab[], int *pNbElt, int Val);

int main(void) {
    int Tab[5] = {2, 5, 9, 5, 8};
    int pMin;
    int pMax;
    int pNb = 5;
    AfficherTab(Tab, pNb);
    printf("Min = %d\n", Min(Tab, pNb));
    printf("Max = %d\n", Max(Tab, pNb));
    MinMax(Tab, pNb, &pMin, &pMax);
    printf("%d min, %d max.\n", pMin, pMax);
    int nb = SupprimerVal(Tab, &pNb, 5);
    printf("%d supprimes\n", nb);
    AfficherTab(Tab, pNb);
    return 0;
}

void AfficherTab(int Tab[], int NbElt) {
    for (int i = 0; i < NbElt; ++i) {
        printf("%d ", Tab[i]);
    }
    printf("\n");
}

int Min(int Tab[], int NbElt) {
    int min = Tab[0];
    for (int j = 1; j < NbElt; ++j) {
        if (Tab[j] < min) {
            min = Tab[j];
        }
    }
    return min;
}

int Max(int Tab[], int NbElt) {
    int max = Tab[0];
    for (int j = 1; j < NbElt; ++j) {
        if (max < Tab[j]) {
            max = Tab[j];
        }
    }
    return max;
}

void MinMax(int Tab[], int NbElt, int *pMin, int *pMax) {
    *pMin = Min(Tab, NbElt);
    *pMax = Max(Tab, NbElt);
}

int SupprimerVal(int Tab[], int *pNbElt, int Val) {
    int nbSuppr = 0;
    for (int i = 0; i < *pNbElt; i++) {
        if (Tab[i] == Val) {
            for (int j = i; j < *pNbElt - 1; j++) {
                Tab[j] = Tab[j + 1];
            }
            (*pNbElt)--;
            nbSuppr++;
            i--; // Recheck the current index in case of duplicates
        }
    }
    return nbSuppr;
}
