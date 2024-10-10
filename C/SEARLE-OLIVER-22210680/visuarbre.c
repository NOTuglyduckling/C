#include "genea.h"

int main(int argc, char *argv[]){
    if (argc != 4) {
        fprintf(stderr, "Usage: %s fichier-personnes fichier-liens-parente fichier-dot\n", argv[0]);
        return 1;
    }
    
    char *fichierPersonnes = argv[1];
    char *fichierLiensParente = argv[2];
    char *fichierDot = argv[3];

    tArbre arbre = ArbreCreer();
    if (arbre == NULL) {
        fprintf(stderr, "Erreur lors de la création de l'arbre.\n");
        return 1;
    }

    // Lire les personnes et les liens de parenté à partir des fichiers
    arbre = ArbreLirePersonnesFichier(fichierPersonnes);
    if (arbre == NULL) {
        fprintf(stderr, "Erreur lors de la lecture des personnes depuis le fichier %s.\n", fichierPersonnes);
        return 1;
    }

    arbre = ArbreLireLienParenteFichier(arbre, fichierLiensParente);
    if (arbre == NULL) {
        fprintf(stderr, "Erreur lors de la lecture des liens de parenté depuis le fichier %s.\n", fichierLiensParente);
        return 1;
    }

    // Afficher l'arbre
    printf("Arbre genealogique :\n");
    ArbreAfficher(arbre);

    // Écrire l'arbre généalogique au format DOT
    ArbreEcrireGV(arbre, fichierDot);

    // Libérer l'arbre
    ArbreLiberer(arbre);

    return 0;
}