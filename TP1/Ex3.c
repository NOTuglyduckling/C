#include <stdio.h>

int main() {
    int nombre, carre;

    printf("Veuillez entrer un nombre entier : ");
    scanf("%d", &nombre);

    carre = nombre * nombre;

    printf("Le carré de %d est : %d\n", nombre, carre);

    return 0;
}
