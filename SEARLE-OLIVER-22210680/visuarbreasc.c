#include "genea.h"

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s fichier-personnes fichier-liens-parente identifiant fichier-dot\n", argv[0]);
        return 1;
    }

    char *fichierPersonnes = argv[1];
    char *fichierLiensParente = argv[2];
    int identifiant = atoi(argv[3]);
    char *fichierDot = argv[4];

    // Lire l'arbre à partir des fichiers
    tArbre arbre = ArbreLirePersonnesFichier(fichierPersonnes);
    arbre = ArbreLireLienParenteFichier(arbre, fichierLiensParente);

    // Afficher l'arbre ascendant
    printf("Arbre genealogique ascendant de l'identifiant %d :\n", identifiant);
    ArbreAfficherAscendants(arbre, identifiant);

    // Écrire l'arbre ascendant au format DOT
    ArbreEcrireAscendantsGV(arbre, identifiant, fichierDot);

    // Libérer la mémoire
    ArbreLiberer(arbre);

    return 0;
}
