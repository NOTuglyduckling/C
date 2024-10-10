#include <stdio.h>
#include <stdlib.h>

void Trier(int Tab[], int NbElts);
void Aff(int Tab[], int NbElts);
int* TrierPtr(int Tab[], int NbElts);
void AffPtr(int* TabP, int NbElts);

int main(void) {
    int Tab[5] = {3, 2, 7, 4, 0};
    int* PTab = TrierPtr(Tab, 5);
    Trier(Tab, 5);
    AffPtr(PTab, 5);
    Aff(Tab, 5);
    
    free(PTab); // Don't forget to free the memory allocated by TrierPtr
    return 0;
}

void Trier(int Tab[], int NbElts) {
    int swapped;
    do {
        swapped = 0;
        for (int i = 1; i < NbElts; i++) {
            if (Tab[i - 1] > Tab[i]) {
                int temp = Tab[i - 1];
                Tab[i - 1] = Tab[i];
                Tab[i] = temp;
                swapped = 1;
            }
        }
    } while (swapped);
}

void Aff(int Tab[], int NbElts) {
    for (int i = 0; i < NbElts; i++) {
        printf("%d ", Tab[i]);
    }
    printf("\n");
}

int* TrierPtr(int Tab[], int NbElts) {
    int* PTab = malloc(NbElts * sizeof(int));
    if (PTab == NULL) {
        printf("Erreur d'allocation memoire\n");
        return NULL;
    }
    for (int i = 0; i < NbElts; i++) {
        PTab[i] = Tab[i];
    }
    Trier(PTab, NbElts);
    return PTab;
}

void AffPtr(int* TabP, int NbElts) {
    for (int i = 0; i < NbElts; i++) {
        printf("%d ", TabP[i]); // Use %d to print integers, not %p
    }
    printf("\n");
}
