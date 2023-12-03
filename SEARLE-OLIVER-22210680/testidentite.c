#include "identite.h"

int main() {
    // Exemple d'utilisation de la fonction IdentiteCreer
    tIdentite personne = IdentiteCreer(1, "Doe", "John", 'M', "01/01/2000");

    // Vérification de l'allocation mémoire
    if (personne != NULL) {
        // Affichage des informations
        printf("Identifiant: %d\n", personne->Identifiant);
        printf("Nom: %s\n", personne->Nom);
        printf("Prenom: %s\n", personne->Prenom);
        printf("Sexe: %c\n", personne->Sexe);
        printf("Date de naissance: %s\n", personne->DateNaissance);

        // Libération de la mémoire allouée dynamiquement
        free(personne->Nom);
        free(personne->Prenom);
        free(personne);
    } else {
        printf("Erreur d'allocation mémoire.\n");
    }

    return 0;
}