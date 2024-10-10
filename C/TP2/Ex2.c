#include <stdio.h>

int main() {
    char caractere;
    int nombreLettres = 0;
    int nombreVoyelles = 0;

    printf("Entrez un texte (terminez par #) : ");

    // Lire les caractères jusqu'à rencontrer #
    while ((caractere = getchar()) != '#') {
        // Vérifier si le caractère est une lettre minuscule non accentuée
        if ((caractere >= 'a' && caractere <= 'z')) {
            nombreLettres++;

            // Vérifier si le caractère est une voyelle
            if (caractere == 'a' || caractere == 'e' || caractere == 'i' || caractere == 'o' || caractere == 'u') {
                nombreVoyelles++;
            }
        }
    }

    printf("Nombre de lettres : %d\n", nombreLettres);
    printf("Nombre de voyelles : %d\n", nombreVoyelles);

    return 0;
}
