#include <stdio.h>

int main() {
    int fahrenheit, celsius;

    printf("Veuillez entrer la température en degrés Fahrenheit (entier) : ");
    scanf("%d", &fahrenheit);

    // Calcul de la température en degrés Celsius
    celsius = (5 * (fahrenheit - 32)) / 9;

    printf("La température en degrés Celsius est : %d\n", celsius);

    return 0;
}
