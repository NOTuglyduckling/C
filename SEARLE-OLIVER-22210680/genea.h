#ifndef GENEA_H
#define GENEA_H
#include "identite.h"

// Arbre généalogique
struct sArbre
{
struct sFiche * pPremiere ; // Adresse de la première fiche
struct sFiche * pDerniere ; // Adresse de la dernière fiche
};

// Fiche associée à chaque individu présent dans l’arbre
struct sFiche
{
tIdentite Identite ; // Accès aux informations de l’identité de la personne
struct sFiche * pPere ; // Adresse de la fiche du père
struct sFiche * pMere ; // Adresse de la fiche de la mère
struct sFiche * pSuivante ; // Adresse de la fiche suivante
};

// Type permettant de manipuler un arbre généalogique
typedef struct sArbre * tArbre ;

// Toutes les fonctions dans genea.c
extern tArbre ArbreCreer(void);
extern void ArbreAfficher(tArbre Arbre);
extern void ArbreAjouterPersonne(tArbre Arbre, tIdentite Identite);
extern void ArbreLiberer(tArbre Arbre);
extern tArbre ArbreLirePersonnesFichier(char Fichier[]);
extern void ArbreAjouterLienParente(tArbre Arbre, int IdEnfant, int IdParent, char Parente);
extern int ArbreLireLienParentef(FILE *f, int *pIdEnfant, int *pIdParent, char *pParente);
extern tArbre ArbreLireLienParenteFichier(tArbre Arbre, char Fichier[]);
extern void ArbreEcrireGV(tArbre Arbre, char Fichier[]);
extern void ArbreAfficherAscendants(tArbre Arbre, int Identifiant);
extern void AfficherAscendantsRecursive(tArbre Arbre, struct sFiche *personne, int niveau);
extern void ArbreEcrireAscendantsGV(tArbre Arbre, int Identifiant, char Fichier[]);
extern void EcrireAscendantsGVRecursive(FILE *f, tArbre Arbre, struct sFiche *personne);

#endif // GENEA_H