#include <stdio.h>

int main() {
    char texte[81]; // Tableau pour stocker la ligne de texte (80 caractères max + 1 pour '\0')
    int longueur = 0; // Pour compter le nombre de caractères

    printf("Entrez une ligne de texte (max. 80 caractères) : ");

    // Lire caractère par caractère jusqu'à '\n' ou jusqu'à 80 caractères
    char caractere;
    while ((caractere = getchar()) != '\n' && longueur < 80) {
        texte[longueur] = caractere;
        longueur++;
    }

    // Ajouter le caractère nul pour terminer la chaîne
    texte[longueur] = '\0';

    // Afficher la ligne de texte et sa longueur
    printf("Ligne de texte : %s\n", texte);
    printf("Nombre de caractères : %d\n", longueur);

    return 0;
}
