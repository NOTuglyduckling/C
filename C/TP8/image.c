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
    int lig;
    fscanf(Im,"%d",&lig);
    return lig;
}

int ImNbCol(tImage Im) {
    int col;
    fscanf(Im,"%d",&col);
    return col;
}

tMatrice ImNivGris(tImage Im) {

    int NbLignes = ImNbLig(Im);
    int NbColonnes = ImNbCol(Im);
    int NivGris = 
    tImage image = I(NbLignes,NbColonnes);
    return image;
}

tImage ImLire (char NomFichier[]){
    FILE *Im = fopen(NomFichier,"rt");
    if (Im == NULL){
        printf("Erreur ouverture fichier");
        return 1;
    }
    char magic[3];
    fscanf(Im, "%2s", magic);
    if (magic != "P2"){
        printf("Le fichier n'est pas un fichier PGM");
        return 2;
    }
    // Ignorer les commentaires
    char commentaire[100];
    while (fgetc(Im) == '#') {
    fgets(commentaire, sizeof(commentaire), Im);
    }

    int NbLig = ImNbLig(Im);
    int NbCol = ImNbCol(Im);

    fclose(Im);
    return Im;
}

void ImEcrire(tImage Im, char NomFichier[]){

}