#include <stdio.h>

int main() {
    int nombreDeNotes;
    double somme = 0.0;

    printf("Combien de notes souhaitez-vous entrer ? ");
    scanf("%d", &nombreDeNotes);

    if (nombreDeNotes <= 0) {
        printf("Le nombre de notes doit être supérieur à zéro.\n");
        return 1;
    }

    printf("Veuillez entrer les %d notes (comprises entre 0 et 20) :\n", nombreDeNotes);

    for (int i = 0; i < nombreDeNotes; i++) {
        double note;

        do {
            printf("Note %d : ", i + 1);
            scanf("%lf", &note);

            if (note < 0 || note > 20) {
                printf("La note doit être comprise entre 0 et 20. Veuillez réessayer.\n");
            }
        } while (note < 0 || note > 20);

        somme += note;
    }

    double moyenne = somme / nombreDeNotes;
    printf("La moyenne des %d notes est : %.2lf\n", nombreDeNotes, moyenne);

    return 0;
}
