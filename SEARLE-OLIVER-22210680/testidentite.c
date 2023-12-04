#include "identite.h"

int main() {
    // Test IdentiteCreer function
    tIdentite identite1 = IdentiteCreer(1, "Doe", "John", 'M', "01/01/1990");
    tIdentite identite2 = IdentiteCreer(2, "Smith", "Jane", 'F', "15/05/1985");

    // Test IdentiteAfficher function
    printf("Identity 1:\n");
    IdentiteAfficher(identite1);

    printf("\nIdentity 2:\n");
    IdentiteAfficher(identite2);

    // Test IdentiteLiref function
    FILE *file = fopen("personne.ind", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }

    tIdentite identiteFromFile = IdentiteLiref(file);
    fclose(file);

    // Check if reading from file was successful
    if (identiteFromFile == NULL) {
        fprintf(stderr, "Error reading identity from file.\n");
        return 1;
    }

    // Test IdentiteAfficher function for the identity read from file
    printf("\nIdentity from file:\n");
    IdentiteAfficher(identiteFromFile);

    // Test IdentiteLiberer function
    IdentiteLiberer(identite1);
    IdentiteLiberer(identite2);
    IdentiteLiberer(identiteFromFile);

    return 0;
}