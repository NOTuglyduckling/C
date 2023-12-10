#include "genea.h"

tArbre ArbreCreer(void){
    tArbre Arbre = malloc(sizeof(struct sArbre));
    if (Arbre == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour l'arbre.\n");
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
        printf("\tPere : ");
        if (current->pPere != NULL) {
            IdentiteAfficher(current->pPere->Identite);
        } else {
            printf("inconnu\n");
        }

        // Afficher la mère
        printf("\tMere : ");
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
        return;
    }
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
        return NULL;
    }
    tArbre arbre = ArbreCreer();
    if (arbre == NULL) {
        fclose(f);
        return NULL;
    }
    // Lire chaque personne depuis le fichier et les ajouter à l'arbre
    while (1) {
        tIdentite nouvelleIdentite = IdentiteLiref(f);
        if (nouvelleIdentite == NULL) {
            break;
        }
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
        fprintf(stderr, "Erreur : Identifiant non trouve dans l'arbre.\n");
        return;
    }

    // Établir le lien de parenté en fonction du type de parenté
    if (Parente == 'P') {
        enfant->pPere = parent;
    } else if (Parente == 'M') {
        enfant->pMere = parent;
    } else {
        fprintf(stderr, "Erreur : Type de parente non valide.\n");
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
    FILE *f = fopen(Fichier, "r");
    if (f == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier %s.\n", Fichier);
        return NULL;
    }

    int idEnfant, idParent;
    char parente;

    // Lire les liens de parenté à partir du fichier
    while (ArbreLireLienParentef(f, &idEnfant, &idParent, &parente)) {
        // Ajouter le lien de parenté à l'arbre
        ArbreAjouterLienParente(Arbre, idEnfant, idParent, parente);
    }

    fclose(f);
    return Arbre;
}

void ArbreEcrireGV(tArbre Arbre, char Fichier[]) {
    FILE *f = fopen(Fichier, "w");
    if (f == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier %s pour écriture.\n", Fichier);
        return;
    }

    // Écriture de l'en-tête DOT
    fprintf(f, "digraph {\n");
    fprintf(f, "\trankdir = \"BT\";\n");

    // Écriture des styles des nœuds et la couleur pour les hommes
    fprintf(f, "\n\tnode [shape = box, color = blue, fontname = \"Arial\", fontsize = 10];\n");

    // Parcourir l'arbre pour écrire les nœuds pour les hommes
    struct sFiche* current = Arbre->pPremiere;
    while (current != NULL) {
        if (IdentiteSexe(current->Identite)=='M'){
        fprintf(f, "\t%d [label=\"%s\\n%s\\n%s\"];\n",
                IdentiteIdentifiant(current->Identite),
                IdentiteNom(current->Identite),
                IdentitePrenom(current->Identite),
                IdentiteDateNaissance(current->Identite));
        }
        current = current->pSuivante;
    }

    // Changement de couleur pour les femmes
    fprintf(f, "\n\tnode [color = green];\n");

    // Parcourir l'arbre pour écrire les nœuds pour les femmes
    current = Arbre->pPremiere;
    while (current != NULL) {
        if (IdentiteSexe(current->Identite)=='F') {
            fprintf(f, "\t%d [label=\"%s\\n%s\\n%s\"];\n",
                   IdentiteIdentifiant(current->Identite),
                    IdentiteNom(current->Identite),
                    IdentitePrenom(current->Identite),
                    IdentiteDateNaissance(current->Identite));
        }
        current = current->pSuivante;
    }

    // Écriture du style des arcs du graphe
    fprintf(f, "\n\tedge [dir = none];\n");

    // Parcourir l'arbre pour écrire les arcs
    current = Arbre->pPremiere;
    while (current != NULL) {
        if (current->pPere != NULL) {
            fprintf(f, "\t%d -> %d;\n",
                    IdentiteIdentifiant(current->Identite),
                    IdentiteIdentifiant(current->pPere->Identite));
        }
        if (current->pMere != NULL) {
            fprintf(f, "\t%d -> %d;\n",
                    IdentiteIdentifiant(current->Identite),
                    IdentiteIdentifiant(current->pMere->Identite));
        }
        current = current->pSuivante;
    }

    // Écriture de la fermeture du graphe
    fprintf(f, "}\n");

    // Fermeture du fichier
    fclose(f);

    printf("Le fichier DOT a été généré avec succès : %s\n", Fichier);
}

void AfficherAscendantsRecursive(tArbre Arbre, struct sFiche *personne, int niveau) {
    // Afficher l'identité de la personne avec le niveau approprié de tabulations
    for (int i = 0; i < niveau; i++) {
        printf("\t");
    }
    IdentiteAfficher(personne->Identite);

    // Afficher les ascendants
    if (personne->pPere != NULL) {
        // Afficher le nombre correct de tabulations
        for (int i = 0; i < niveau + 1; i++) {
            printf("\t");
        }
        printf("Père : ");
        //appeler la fonction récursive
        AfficherAscendantsRecursive(Arbre, personne->pPere, niveau + 1);
    }

    if (personne->pMere != NULL) {
        // Afficher le nombre correct de tabulations
        for (int i = 0; i < niveau + 1; i++) {
            printf("\t");
        }
        printf("Mère : ");
        //appeler la fonction récursive
        AfficherAscendantsRecursive(Arbre, personne->pMere, niveau + 1);
    }
}

void ArbreAfficherAscendants(tArbre Arbre, int Identifiant) {
    struct sFiche *personne = NULL;
    struct sFiche *current = Arbre->pPremiere;

    // Trouver la personne dans l'arbre
    while (current != NULL) {
        if (IdentiteIdentifiant(current->Identite) == Identifiant) {
            personne = current;
            break;
        }
        current = current->pSuivante;
    }

    // Vérifier si la personne a été trouvée
    if (personne == NULL) {
        fprintf(stderr, "Erreur : Identifiant %d non trouvé dans l'arbre.\n", Identifiant);
        return;
    }

    // Appeler la fonction récursive avec le niveau initial 0
    AfficherAscendantsRecursive(Arbre, personne, 0);
}

void EcrireAscendantsGVRecursive(FILE *f, tArbre Arbre, struct sFiche *personne) {

    if (IdentiteSexe(personne->Identite)=='M'){
        fprintf(f, "\tnode [color = blue];\n");
    } else {
        fprintf(f, "\tnode [color = green];\n");
    } // Change la couleur d'affichage selon le sexe

    fprintf(f, "%d [label=\"%s\\n%s\\n%s\", shape=box];\n",
            IdentiteIdentifiant(personne->Identite),
            IdentiteNom(personne->Identite),
            IdentitePrenom(personne->Identite),
            IdentiteDateNaissance(personne->Identite));

    // Écrire les arcs vers les ascendants
    if (personne->pPere != NULL) {
        fprintf(f, "\t%d -> %d [label=\"Père\"];\n",
                IdentiteIdentifiant(personne->Identite),
                IdentiteIdentifiant(personne->pPere->Identite));
        EcrireAscendantsGVRecursive(f, Arbre, personne->pPere);
    }

    if (personne->pMere != NULL) {
        fprintf(f, "\t%d -> %d [label=\"Mère\"];\n",
                IdentiteIdentifiant(personne->Identite),
                IdentiteIdentifiant(personne->pMere->Identite));
        EcrireAscendantsGVRecursive(f, Arbre, personne->pMere);
    }
}

void ArbreEcrireAscendantsGV(tArbre Arbre, int Identifiant, char Fichier[]) {
    FILE *f = fopen(Fichier, "w");
    if (f == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier %s.\n", Fichier);
        return;
    }

    // Écrire l'en-tête du fichier DOT
    fprintf(f, "digraph {\n");
    fprintf(f, "\trankdir = \"BT\";\n");
    // Écriture du style d'écriture
    fprintf(f, "\tnode [shape = box, fontname = \"Arial\", fontsize = 10];\n");
    // Écriture du style des arcs du graphe
    fprintf(f, "\tedge [dir = none];\n");

    // Trouver la personne dans l'arbre
    struct sFiche *personne = NULL;
    struct sFiche *current = Arbre->pPremiere;

    while (current != NULL) {
        if (IdentiteIdentifiant(current->Identite) == Identifiant) {
            personne = current;
            break;
        }
        current = current->pSuivante;
    }

    // Vérifier si la personne a été trouvée
    if (personne == NULL) {
        fprintf(stderr, "Erreur : Identifiant %d non trouvé dans l'arbre.\n", Identifiant);
        fclose(f);
        return;
    }

    // Appeler la fonction récursive pour écrire les ascendants
    EcrireAscendantsGVRecursive(f, Arbre, personne);

    // Écrire la fermeture du fichier DOT
    fprintf(f, "}\n");

    fclose(f);
}

