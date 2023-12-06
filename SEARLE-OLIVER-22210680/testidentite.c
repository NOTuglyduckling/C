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
    FILE *file = fopen("arbre4.ind", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }

    tIdentite identiteFromFile1 = IdentiteLiref(file);
    tIdentite identiteFromFile2 = IdentiteLiref(file);
    tIdentite identiteFromFile3 = IdentiteLiref(file);
    fclose(file);

    // Check if reading from file was successful
    // if (identiteFromFile1 == NULL) {
    //     fprintf(stderr, "Error reading identity 1 from file.\n");
    //     return 1;
    // }
    // if (identiteFromFile2 == NULL) {
    //     fprintf(stderr, "Error reading identity 2 from file.\n");
    //     return 1;
    // }
    // if (identiteFromFile3 == NULL) {
    //     fprintf(stderr, "Error reading identity 3 from file.\n");
    //     return 1;
    // }

    // Test IdentiteAfficher function for the identity read from file
    printf("\nIdentity from file:\n");
    IdentiteAfficher(identiteFromFile1);
    IdentiteAfficher(identiteFromFile2);
    IdentiteAfficher(identiteFromFile3);

    // Test IdentiteLiberer function
    IdentiteLiberer(identite1);
    IdentiteLiberer(identite2);
    IdentiteLiberer(identiteFromFile1);
    IdentiteLiberer(identiteFromFile2);
    IdentiteLiberer(identiteFromFile3);

    return 0;
}