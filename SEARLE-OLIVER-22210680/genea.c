#include "genea.h"

tArbre ArbreCreer(void){
    tArbre Arbre = malloc(sizeof(struct sArbre));
    if (Arbre == NULL) {
        return NULL;}
    
    Arbre->pPremiere = NULL;
    Arbre->pDerniere = NULL;

    return Arbre;    
}

void ArbreAfficher(tArbre Arbre){
    struct sFiche *current = Arbre->pPremiere;

    while (current != NULL) {
        // Afficher l'identité de la personne
        IdentiteAfficher(current->Identite);

        // Afficher le père
        printf("\tPère : ");
        if (current->pPere != NULL) {
            IdentiteAfficher(current->pPere->Identite);
        } else {
            printf("inconnu\n");
        }

        // Afficher la mère
        printf("\tMère : ");
        if (current->pMere != NULL) {
            IdentiteAfficher(current->pMere->Identite);
        } else {
            printf("inconnue\n");
        }

        // Aller à la fiche suivante
        current = current->pSuivante;
    }
}

void ArbreAjouterPersonne(tArbre Arbre, tIdentite Identite){
    struct sFiche* nouvelleFiche = malloc(sizeof(struct sFiche));
    if (nouvelleFiche == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour la nouvelle personne.\n");
        return;}

    // Initialiser les champs de la fiche
    nouvelleFiche->Identite = Identite;
    nouvelleFiche->pPere = NULL;
    nouvelleFiche->pMere = NULL;
    nouvelleFiche->pSuivante = NULL;

    // Ajouter la fiche à la fin de la liste
    if (Arbre->pDerniere == NULL) {
        // Si l'arbre est vide, la nouvelle fiche devient la première et la dernière
        Arbre->pPremiere = nouvelleFiche;
        Arbre->pDerniere = nouvelleFiche;
    } else {
        // Sinon, ajouter la nouvelle fiche à la fin de la liste
        Arbre->pDerniere->pSuivante = nouvelleFiche;
        Arbre->pDerniere = nouvelleFiche;
    }
}

void ArbreLiberer(tArbre Arbre){
    struct sFiche* current = Arbre->pPremiere;
    while (current != NULL) {
        struct sFiche* next = current->pSuivante;

        // Libérer l'identité associée à la fiche
        IdentiteLiberer(current->Identite);

        // Libérer la fiche actuelle
        free(current);

        // Passer à la fiche suivante
        current = next;
    }

    // Libérer la structure sArbre
    free(Arbre);
}

tArbre ArbreLirePersonnesFichier(char Fichier[]){
    FILE *f = fopen(Fichier, "r");
    if (f == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier %s.\n", Fichier);
        return NULL;}

    tArbre arbre = ArbreCreer();
    if (arbre == NULL) {
        fclose(f);
        return NULL;
    }

    // Lire chaque personne depuis le fichier et les ajouter à l'arbre
    while (1) {
        tIdentite nouvelleIdentite = IdentiteLiref(f);
        if (nouvelleIdentite == NULL) {
            break;}

        ArbreAjouterPersonne(arbre, nouvelleIdentite);
    }

    fclose(f);
    return arbre;
}

void ArbreAjouterLienParente(tArbre Arbre, int IdEnfant, int IdParent, char Parente){
    struct sFiche *enfant = NULL;
    struct sFiche *parent = NULL;

    // Commencer au début de l'arbre, chercher l'enfant
    struct sFiche *current = Arbre->pPremiere;
    while (current != NULL) {
        if (IdentiteIdentifiant(current->Identite) == IdEnfant) {
            enfant = current;
            break;
        }
        current = current->pSuivante;
    }

    // Recommencer au début de l'arbre, chercher le parent
    current = Arbre->pPremiere;
    while (current != NULL) {
        if (IdentiteIdentifiant(current->Identite) == IdParent) {
            parent = current;
            break;
        }
        current = current->pSuivante;
    }

    // Vérifier si les identifiants sont trouvés
    if (enfant == NULL || parent == NULL) {
        fprintf(stderr, "Erreur : Identifiant non trouvé dans l'arbre.\n");
        return;
    }

    // Établir le lien de parenté en fonction du type de parenté
    if (Parente == 'P') {
        enfant->pPere = parent;
    } else if (Parente == 'M') {
        enfant->pMere = parent;
    } else {
        fprintf(stderr, "Erreur : Type de parenté non valide.\n");
    }
}

int ArbreLireLienParentef(FILE *f, int *pIdEnfant, int *pIdParent, char *pParente){
    if (fscanf(f, "%d %d %c", pIdEnfant, pIdParent, pParente) == 3) {
        return 1; // La lecture s'est bien passée
    } else {
        return 0;
    }
}

tArbre ArbreLireLienParenteFichier(tArbre Arbre, char Fichier[]){

}

