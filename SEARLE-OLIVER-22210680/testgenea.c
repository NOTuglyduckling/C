#include "genea.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s fichier-personnes fichier-liens-parente\n", argv[0]);
        return 1;
    }

    char *fichierPersonnes = argv[1];
    char *fichierLiensParente = argv[2];

    // Créer un arbre vide
    tArbre arbre = ArbreCreer();
    if (arbre == NULL) {
        fprintf(stderr, "Erreur lors de la création de l'arbre.\n");
        return 1;
    }

    // Ajouter les personnes à l'arbre depuis le fichier de personnes
    arbre = ArbreLirePersonnesFichier(fichierPersonnes);
    if (arbre == NULL) {
        fprintf(stderr, "Erreur lors de la lecture des personnes depuis le fichier.\n");
        return 1;
    }

    // Ajouter les liens de parenté à l'arbre depuis le fichier de liens de parenté
    arbre = ArbreLireLienParenteFichier(arbre, fichierLiensParente);
    if (arbre == NULL) {
        fprintf(stderr, "Erreur lors de la lecture des liens de parenté depuis le fichier.\n");
        return 1;
    }

    // Afficher l'arbre
    printf("Arbre généalogique :\n");
    ArbreAfficher(arbre);

    // Libérer la mémoire allouée pour l'arbre
    ArbreLiberer(arbre);

    return 0;
}