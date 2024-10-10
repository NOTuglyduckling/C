#include <stdio.h>

int main() {
    int nombreDeValeurs;
    double somme = 0.0;

    printf("Entrez le nombre de valeurs (maximum 100) : ");
    scanf("%d", &nombreDeValeurs);

    if (nombreDeValeurs <= 0 || nombreDeValeurs > 100) {
        printf("Le nombre de valeurs doit être compris entre 1 et 100.\n");
        return 1;
    }

    double valeurs[nombreDeValeurs];
    
    // Saisir les valeurs et calculer la somme
    for (int i = 0; i < nombreDeValeurs; i++) {
        printf("Entrez la valeur %d : ", i + 1);
        scanf("%lf", &valeurs[i]);
        somme += valeurs[i];
    }

    double moyenne = somme / nombreDeValeurs;
    printf("La moyenne est : %.2lf\n", moyenne);

    printf("Les valeurs strictement supérieures à la moyenne sont : ");
    for (int i = 0; i < nombreDeValeurs; i++) {
        if (valeurs[i] > moyenne) {
            printf("%.2lf ", valeurs[i]);
        }
    }
    printf("\n");

    return 0;
}
