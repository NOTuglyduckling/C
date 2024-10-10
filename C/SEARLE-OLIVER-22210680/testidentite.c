#include "identite.h"

int main() {
    // Test fonction IdentiteCreer
    tIdentite identite1 = IdentiteCreer(1, "Searle", "Oliver", 'M', "07/01/2003");
    tIdentite identite2 = IdentiteCreer(2, "Smith", "Jane", 'F', "15/05/1985");

    // Test fonction IdentiteAfficher
    printf("Identite 1:\n");
    IdentiteAfficher(identite1);

    printf("\nIdentite 2:\n");
    IdentiteAfficher(identite2);

    // Test fonction IdentiteLiref
    FILE *file = fopen("personne.ind", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }

    tIdentite identiteFromFile = IdentiteLiref(file);

    fclose(file);

    printf("\nIdentite du fichier:\n");
    // Verifie lecture correcte
    if (identiteFromFile == NULL) {
        fprintf(stderr, "Error reading identity 1 from file.\n");
        return 1;
    }

    // Test fonction IdentiteAfficher pour l'identité du fichier
    IdentiteAfficher(identiteFromFile);


    // Test fonction IdentiteLiberer
    IdentiteLiberer(identite1);
    IdentiteLiberer(identite2);
    IdentiteLiberer(identiteFromFile);

    return 0;
}