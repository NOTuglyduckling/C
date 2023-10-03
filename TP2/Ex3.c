#include <stdio.h>

int main() {
    int nombreDeValeurs;
    double somme = 0.0;

    printf("Combien de valeurs souhaitez-vous entrer ? ");
    scanf("%d", &nombreDeValeurs);

    if (nombreDeValeurs <= 0) {
        printf("Le nombre de valeurs doit être supérieur à zéro.\n");
        return 1;
    }

    printf("Veuillez entrer les %d valeurs :\n", nombreDeValeurs);

    for (int i = 0; i < nombreDeValeurs; i++) {
        double valeur;
        printf("Valeur %d : ", i + 1);
        scanf("%lf", &valeur);
        somme += valeur;
    }

    double moyenne = somme / nombreDeValeurs;
    printf("La moyenne des %d valeurs est : %.2lf\n", nombreDeValeurs, moyenne);

    return 0;
}
