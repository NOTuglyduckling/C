#include <stdio.h>
#include <ctype.h> // Inclure la bibliothèque ctype pour utiliser islower

int main() {
    char caractere;
    int nombreLettres = 0;
    int nombreVoyelles = 0;

    printf("Entrez un texte (terminez par #) : ");

    // Lire les caractères jusqu'à rencontrer #
    while ((caractere = getchar()) != '#') {
        // Vérifier si le caractère est une lettre minuscule non accentuée ou un espace
        if (islower(caractere) || caractere == ' ') {
            nombreLettres++;

            // Vérifier si le caractère est une voyelle
            if (caractere == 'a' || caractere == 'e' || caractere == 'i' || caractere == 'o' || caractere == 'u') {
                nombreVoyelles++;
            }
        } else {
            // Caractère invalide
            printf("Caractère invalide : '%c' (Code ASCII : %d)\n", caractere, caractere);
            return 1; // Sortir du programme en cas de caractère invalide
        }
    }

    printf("Nombre de lettres : %d\n", nombreLettres);
    printf("Nombre de voyelles : %d\n", nombreVoyelles);

    return 0;
}
