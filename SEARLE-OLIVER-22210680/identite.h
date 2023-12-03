#ifndef IDENTITE_H
#define IDENTITE_H

#define LG_DATE 10 
// Nombre de caract è res d ’ une date sous la forme jj / mm / aaaa
// Identit é d ’ une personne
struct sIdentite
{
int Identifiant ; // Identifiant unique
char * Nom ; // Nom ( cha î ne de caract è res allou é e dynamiquement )
char * Prenom ; // Pr é nom ( cha î ne de caract è res allou é e dynamiquement )
char Sexe ; // ’F ’ ou ’M ’
char DateNaissance [ LG_DATE +1]; // Date de naissance sous la forme jj / mm / aaaa
};
// Type permettant de manipuler l ’ identit é d ’ une personne
typedef struct sIdentite * tIdentite ;

extern tIdentite IdentiteCreer(int Id, char *Nom, char *Prenom, char Sexe, char DateNais[]);
extern int IdentiteIdentifiant(tIdentite Identite);
extern char *IdentiteNom(tIdentite Identite);
extern char *IdentitePrenom(tIdentite Identite);
extern char IdentiteSexe(tIdentite Identite);
extern char *IdentiteDateNaissance(tIdentite Identite);
extern void IdentiteAfficher(tIdentite Identite);
extern void IdentiteLiberer(tIdentite Identite);
extern tIdentite IdentiteLiref(FILE *f);

#endif // IDENTITE_H