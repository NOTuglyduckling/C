#include "identite.h"

tIdentite IdentiteCreer(int Id, char *Nom, char *Prenom, char Sexe, char DateNais[]){
    tIdentite Identite = malloc(sizeof(struct sIdentite));
    if (Identite == NULL){
        return NULL;}

    Identite->Nom = malloc(strlen(Nom)+1);
    if (Identite->Nom == NULL){
        free(Identite);
        return NULL;}

    Identite->Prenom = malloc(strlen(Prenom)+1);
    if (Identite->Prenom == NULL){
        free(Identite->Nom);
        free(Identite);
        return NULL;}

    // Si l'allocation mémoire marche bien on commence à ajouter les valeurs.

    Identite->Identifiant = Id;
    strcpy(Identite->Nom, Nom);
    strcpy(Identite->Prenom, Prenom);
    Identite->Sexe = Sexe;
    strcpy(Identite->DateNaissance, DateNais);
    return Identite;
}

int IdentiteIdentifiant(tIdentite Identite){
    return Identite->Identifiant;
}

char *IdentiteNom(tIdentite Identite){
    return Identite->Nom;
}

char *IdentitePrenom(tIdentite Identite){
    return Identite->Prenom;
}

char IdentiteSexe(tIdentite Identite){
    return Identite->Sexe;
}

char *IdentiteDateNaissance(tIdentite Identite){
    return Identite->DateNaissance;
}

void IdentiteAfficher(tIdentite Identite){
    printf("[%d] ", Identite->Identifiant);
    printf("%s ", Identite->Nom);
    printf("%s, ", Identite->Prenom);
    printf("%c, ", Identite->Sexe);
    printf("%s\n", Identite->DateNaissance);
}

void IdentiteLiberer(tIdentite Identite){
    free(Identite->Nom);
    free(Identite->Prenom);
    free(Identite);
}

tIdentite IdentiteLiref(FILE *f){
    int Id;
    char* Nom = malloc(MAX_LINE_LEN);
    char* Prenom = malloc(MAX_LINE_LEN);
    char Sexe;
    char DateNais[LG_DATE + 1];
    
    if (Nom == NULL || Prenom == NULL){
        free(Nom);
        free(Prenom);
        return NULL;}

    // Lecture de l'identifiant
    if (fscanf(f, "%d\n", &Id) != 1){
        // Gérer l'erreur ou la fin du fichier
        free(Nom);
        free(Prenom);
        return NULL;}

    // Lecture du nom
    fgets(Nom, sizeof(Nom), f); // Prends toute la ligne
    Nom[strcspn(Nom, "\n")] = '\0'; // Enlève tout le vide

    // Lecture du prénom
    fgets(Prenom, sizeof(Prenom), f); // Prends toute la ligne
    Prenom[strcspn(Prenom, "\n")] = '\0'; // Enlève tout le vide

    // Lecture du sexe
    fscanf(f, " %c\n", &Sexe);

    // Lecture de la date de naissance
    fgets(DateNais, sizeof(DateNais), f);
    DateNais[strcspn(DateNais, "\n")] = '\0'; // Enlève tout le vide

    tIdentite nouvelleIdentite = IdentiteCreer(Id, Nom, Prenom, Sexe, DateNais);
    
    //libérer les pointeurs temporaires.
    free(Nom);
    free(Prenom);

    return nouvelleIdentite;
}