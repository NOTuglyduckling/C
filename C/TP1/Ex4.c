#include <stdio.h>
#include <math.h> // Inclure la bibliothèque mathématique pour utiliser sqrt

int main() {
    double nombre, racine;

    printf("Veuillez entrer un nombre réel : ");
    scanf("%lf", &nombre);

    // Utilise la fonction sqrt pour calculer la racine carrée
    racine = sqrt(nombre);

    printf("La racine carrée de %.2lf est : %.2lf\n", nombre, racine);

    return 0;
}
