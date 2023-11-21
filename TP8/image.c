#include "image.h"
#include "matrice.h"
#include <stdio.h>
#include <stdlib.h>

struct sImage {
    int NbLig;
    int NbCol;
    tMatrice NivGris;
};

tImage ImAllouer(int NbLignes, int NbColonnes) {
    tImage image = malloc(sizeof(struct sImage));
    if (image == NULL) {
        printf("Erreur d'allocation memoire pour l'image !\n");
        return NULL;
    }

    image->NbLig = NbLignes;
    image->NbCol = NbColonnes;
    image->NivGris = MatAllouer(NbLignes, NbColonnes);

    if (image->NivGris == NULL) {
        printf("Erreur d'allocation memoire pour la matrice des niveaux de gris !\n");
        free(image);
        return NULL;
    }

    return image;
}

void ImLiberer(tImage* pIm) {
    if (*pIm != NULL) {
        MatLiberer(&((*pIm)->NivGris));
        free(*pIm);
        *pIm = NULL;
    }
}

int ImNbLig(tImage Im) {
    return Im->NbLig;
}

int ImNbCol(tImage Im) {
    return Im->NbCol;
}

tMatrice ImNivGris(tImage Im) {
    return Im->NivGris;
}
