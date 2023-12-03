#include "identite.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

tIdentite IdentiteCreer(int Id, char *Nom, char *Prenom, char Sexe, char DateNais[]){
    tIdentite Identite = malloc(sizeof(struct sIdentite));
    if (Identite == NULL) {
        return NULL;}

    Identite->Nom = malloc(strlen(Nom)+1);
    if (Identite->Nom == NULL) {
        free(Identite);
        return NULL;
    }

    Identite->Prenom = malloc(strlen(Prenom)+1);
    if (Identite->Prenom == NULL) {
        free(Identite->Nom);
        free(Identite);
        return NULL;
    }
    // Si l'allocation mémoire marche bien on commence à ajouter les valeurs.

    Identite->Identifiant = Id;
    strcpy(Identite->Nom, Nom);
    strcpy(Identite->Prenom, Prenom);
    Identite->Sexe = Sexe;
    strcpy(Identite->DateNaissance, DateNais);
    return Identite;
}

int IdentiteIdentifiant(tIdentite Identite){

}

char *IdentiteNom(tIdentite Identite){

}

char *IdentitePrenom(tIdentite Identite){

}

char IdentiteSexe(tIdentite Identite){

}

char *IdentiteDateNaissance(tIdentite Identite){

}

void IdentiteAfficher(tIdentite Identite){

}

void IdentiteLiberer(tIdentite Identite){

}

tIdentite IdentiteLiref(FILE *f){

}