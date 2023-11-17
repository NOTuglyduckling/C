#include <stdio.h>
#include <stdlib.h>

/****************************************************************************/
/* SI VOUS SOUHAITEZ COMMENTER LE CORPS D'UNE FONCTION QUE VOUS AVEZ ÉCRIT, */
/* NE COMMENTEZ QUE CE CORPS EN LAISSANT L'EN-TÊTE DE LA FONCTION, LES      */
/* ACCOLADES ET L'ÉVENTUEL RETURN TELS QU'ILS ÉTAIENT INITIALEMENT          */
/****************************************************************************/

// ============================== EXERCICE 1 ============================== //
/* Indice du dernier chiffre contenu dans une chaîne de caractères
 * Paramètre :
 *    - ch : adresse du premier caractère de la chaîne de caractères
 * Valeur de retour :
 *    - indice, dans la chaîne de caractères, du dernier (le plus à droite) caractère
 * qui est un chiffre ou -1 si la chaîne de caractères ne contient aucun chiffre
 */
int IndiceDernierChiffre(const char *ch){
  int i=0;
  int der=-1;
  while (*ch != '\0'){
    if ((*ch >= '0') && (*ch <= '9')){der=i;i++;*ch++;} else {i++;*ch++;}}
  return der; // LIGNE À MODIFIER
}


// ============================== EXERCICE 2 ============================== //
/* Calcul de statistiques sur un tableau de températures :
 *     - moyenne des températures strictement supérieures à une température seuil
 *     - nombre de températures strictement supérieures à une température seuil
 * Paramètres :
 *    - Temp : tableau de températures de type float
 *    - NbTemp : nombre de températures stockées dans le tableau Temp (>= 1)
 *    - Seuil : température seuil
 *    - pMoy : adresse de la moyenne des températures strictement supérieures à Seuil
 * Valeur de retour :
 *    - nombre de températures strictement supérieures à Seuil
 */
int Depassement(float Temp[], int NbTemp, float Seuil, float *pMoy){
  int sup =0;
  float moyenne = 0;
  for(int i =0;i<NbTemp;i++){
    if (Temp[i] > Seuil){
      sup++;
      moyenne += Temp[i];
    }
  }
  moyenne = (moyenne/sup);
  *pMoy = moyenne;
  return sup; // LIGNE À MODIFIER
}

// ============================== EXERCICE 3 ============================== //
/* Taille maximale d'une matrice */
#define NB_LIG_MAX 100
#define NB_COL_MAX 100

/* Calcul des plus petits éléments de chaque ligne d'une matrice de valeurs de type int
 * et du plus petit élément de toute la matrice**pRes = tab
 * Paramètres :
 *    - Mat : matrice (tableau à deux dimensions) de valeurs de type int
 *    - NbLig : nombre de lignes de la matrice (>= 1)
 *    - NbCol : nombre de colonnes de la matrice (>= 1);
 *    - Min : tableau des minimums de chaque ligne de la matrice
 * Valeur de retour :
 *    - minimum de tous les éléments de la matrice
 */
int MinLig(int Mat[NB_LIG_MAX][NB_COL_MAX], int NbLig, int NbCol, int Min[NB_LIG_MAX])
{
    for(int i= 0;i<NbLig;i++){
      int mini = Mat[i][0];
      Min[i] = 0;
      for(int y = 0; y<NbCol;y++){
        if (Mat[i][y] < mini){
          mini = Mat[i][y];
        }
      }
      Min[i] = mini;
    }
    int mini2 = Min[0];
    for (int i = 0; i < NbLig;i++){
      if (Min[i] < mini2){
        mini2 = Min[i];
      }

    }
    return mini2; // LIGNE À MODIFIER
}


// ============================== EXERCICE 4 ============================== //
/* Recopie d'une chaîne de caractères avec suppression des répétitions de caractères
 * Transforme les répétitions d'un même caractère (occurrences multiples successives)
 * en un seul exemplaire de ce caractère. Par exemple, si la source est "bonjjjourr",
 * la destination devra être "bonjour".
 * Paramètres :
 *    - Source : adresse du premier caractère de la chaîne de caractères source
 *    - Destination : adresse du premier caractère de la chaîne de caractères destination
 * Valeur de retour :
 *    - nombre de caractères supprimés (3 dans l'exemple ci-dessus)
 */
int SupprimerRepetitions(const char *source, char *destination){
  if (source == NULL) {
    return -1;}
  int cars = 0;
  char cop = '\0';
  while (*source != '\0') {
    if (*source != cop) {
      *destination = *source;
      destination++;
    } else {cars++;}
    cop = *source;
    source++;
  }
  *destination = '\0';
  return cars;
}


// ============================== EXERCICE 5 ============================== //
/* Calcul de la somme élément par élément de deux tableaux
 * Le tableau résultat doit être alloué dynamiquement
 * Paramètres :
 *    - Tab1 : adresse du premier élément du premier tableau
 *    - Tab2 : adresse du premier élément du second tableau
 *    - NbElts : nombre d'éléments dans chacun des deux tableaux
 * Valeur de retour :
 *    - adresse du tableau résultat alloué dynaiquement par la fonction ou NULL en cas
 *      de problème d'allocation mémoire
 */
double *SommeTab(const double *Tab1, const double *Tab2, int NbElts)
{
  double *tab = malloc(NbElts*sizeof(double));
  for(int i = 0 ; i < NbElts ; i++){
    tab[i]  = Tab1[i] + Tab2[i];
  }
  return tab; // LIGNE À MODIFIER
}


// ============================== EXERCICE 6 ============================== //
/* Extraction d'un élément sur deux d'un tableau d'entiers
 * Cette fonction doit délivrer en sortie un tableau qu'elle aura alloué dynamiquement et
 *  y ranger tous les éléments d'indices pairs (0, 2, 4...) du tableau placé en entrée.
 * On suppose que le nombre d'éléments du tableau placé en entrée est pair.
 * Paramètres :
 *    - Tab : tableau de int
 *    - NbElts : nombre pair d'éléments dans le tableau Tab
 *    - pRes : adresse du pointeur vers le premier élément du tableau résultat qui doit
 *             être alloué dynamiquement par la fonction
 * Valeur de retour :
 *    - Nombre d'éléments rangés dans le tableau résultat
 */
int ExtractionUnSurDeux(int Tab[], int NbElts, int **pRes)
{
  int cpt = 0;
  for(int i = 0; i < NbElts; i++)
    if(i%2 == 0) cpt++;
  int j = 0;
  int * tab = malloc(cpt*sizeof(int));
  for(int i =0; i<NbElts;i++){
    if (i%2==0){
      tab[j]=Tab[i];
      j++;
    }
  }
  *pRes = tab;
  return cpt; // LIGNE À MODIFIER
}


// ============================== EXERCICE 7 ============================== //
/* Type structuré pour décrire un article */
struct sArticle
{
  int Reference; // Référence de l'article
  float Prix;    // Prix de l'article
  int Quantite;     // Nombre d'exemplaires de l'article disponibles dans le stock
};

// ============================== EXERCICE 7.1 ============================== //
/* Création d'un article par allocation dynamique de la mémoire
 * La structure sera allouée dynamiquement
 * Paramètres :
 *    - Ref : référence de l'article créé
 *    - Px : prix de l'article créé
 *    - Qt : nombre d'exemplaires de l'article créé disponibles dans le stock
 * Valeur de retour :
 *    - adresse de l'article créé ou NULL en cas de problème d'allocation mémoire
 */
struct sArticle *CreerArticle(int Ref, float Px, int Qt)
{
    struct sArticle * article = malloc(sizeof(struct sArticle));
    article->Reference = Ref;
    article->Prix = Px;
    article->Quantite = Qt;
    return article; // LIGNE À MODIFIER
}

// ============================== EXERCICE 7.2 ============================== //
/* Récupération des informations sur un article
 * Cette fonction délivre en sortie les information sur un article dont l'adresse est
 * passée en paramètre
 * Paramètres :
 *    - pArticle : adresse de l'article
 *    - pRef : adresse de la référence
 *    - pPx : adresse du prix
 *    - pQt : adresse de la quantité
 */
void InformationsArticle(struct sArticle *pArticle, int *pRef, float *pPx, int *pQt)
{
    *pRef = pArticle->Reference;
    *pPx = pArticle->Prix;
    *pQt = pArticle->Quantite;
}


// ==================== FONCTION D'AFFICHAGE UTILISÉE DANS MAIN ==================== //
/* Affichage des éléments d'un tableau de int
 * Paramètres :
 *    - Tab : tableau de int
 *    - NbElts : nombre d'éléments du tableau Tab
 */
void AffTabInt(int Tab[], int n)
{
  printf("[%d",Tab[0]);
  for (int i=1; i<n; i++)
    printf(", %d",Tab[i]);
  printf("]");
  fflush(stdout);
}


/**************************************************************************/
/* POUR TESTER CHAQUE EXERCICE, "DÉ-COMMENTEZ" LES LIGNES CORRESPONDANTES */
/**************************************************************************/

int main(void)
{

  
  printf("*** TEST DE L'EXERCICE 1 ***\n");
  char ch1[] = "NOV 15A";
  char ch2[] = "NOV 2023";
  char ch3[] = "NOVEMBRE";
  printf("\"%s\" : %d attendu, %d obtenu\n", ch1, 5, IndiceDernierChiffre(ch1));
  printf("\"%s\" : %d attendu, %d obtenu\n", ch2, 7, IndiceDernierChiffre(ch2));
  printf("\"%s\" : %d attendu, %d obtenu\n", ch3, -1, IndiceDernierChiffre(ch3));
  


  {
    printf("\n*** TEST DE L'EXERCICE 2 ***\n");
    float Temperatures[5] = {9.5, 15.0, 14.0, 8.0, 11.0};
    float Moy;
    int NbSup;
    float s;
    s=11.0;
    printf("Seuil égal à %f :\n",s);
    NbSup=Depassement(Temperatures, 5, s, &Moy);
    printf("\tValeurs attendues : %f %d\n", 14.5, 2);
    printf("\tValeurs obtenues  : %f %d\n", Moy, NbSup);
    s=15.0;
    printf("Seuil égal à %f :\n",s);
    NbSup=Depassement(Temperatures, 5, s, &Moy);
    printf("\tValeurs attendue : %d\n", 0);
    printf("\tValeurs obtenue  : %d\n", NbSup);
  }


  {
    printf("\n*** TEST DE L'EXERCICE 3 ***\n");
    int Matrice[NB_LIG_MAX][NB_COL_MAX] = {{5, 8, 3}, {7, 9, 8}};
    int Min, MinLignes[2] = {-2023, -2023};
    Min=MinLig(Matrice, 2, 3, MinLignes);
    printf("Valeurs attendues : [%d, %d] %d\n", 3, 7, 3);
    printf("Valeurs obtenues  : [%d, %d] %d\n", MinLignes[0], MinLignes[1], Min);
  }

  
  {
    printf("\n*** TEST DE L'EXERCICE 4 ***\n");
    char ch1[] = "bonjjjourr";
    char ch2[] = "  12   5";
    char ch3[] = "ab cd";
    char ch[80]="";
    int NbSup;
    NbSup=SupprimerRepetitions(ch1,ch);
    printf("\"%s\" : \"%s\" et %d attendus, \"%s\" et %d obtenus\n", ch1, "bonjour", 3, ch, NbSup);
    NbSup=SupprimerRepetitions(ch2,ch);
    printf("\"%s\" : \"%s\" et %d attendus, \"%s\" et %d obtenus\n", ch2, " 12 5", 3, ch, NbSup);
    NbSup=SupprimerRepetitions(ch3,ch);
    printf("\"%s\" : \"%s\" et %d attendus, \"%s\" et %d obtenus\n", ch3, "ab cd", 0, ch, NbSup);
  }
  

  {
    printf("\n*** TEST DE L'EXERCICE 5 ***\n");
    double t1[3]={10.5,3.2,4.0};
    double t2[3]={1.5,0.3,1.5};
    double *Res=NULL;
    Res=SommeTab(t1,t2,3);
    if (Res==NULL)
      fprintf(stderr,"Problème dans la fonction SommeTab.\n");
    else
    {
      printf("[12.0, 3.5, 5.5] attendu, [%.1f, %.1f, %.1f] obtenu\n",Res[0],Res[1],Res[2]);
      free(Res);
    }
  }


  {
    printf("\n*** TEST DE L'EXERCICE 6 ***\n");
    int t[8]={1,2,3,4,5,6,7,8};
    int *Res=NULL,n;
    n=ExtractionUnSurDeux(t,8,&Res);
    if (Res==NULL)
      fprintf(stderr,"Problème dans la fonction ExtractionUnSurDeux.\n");
    else
    {
      printf("[1, 3, 5, 7] attendu, ");
      fflush(stdout);
      AffTabInt(Res,n);
      printf(" obtenu\n");
      free(Res);
    }
  }


  {
    printf("\n*** TEST DE L'EXERCICE 7 ***\n");
    struct sArticle *pArt;
    pArt=CreerArticle(1526,9.99,500);
    int Ref=0,Qt=0;
    float Px=0.0;
    InformationsArticle(pArt,&Ref,&Px,&Qt);
    printf("{%d, %.2f, %d} attendu, {%d, %.2f, %d} obtenu\n",1526,9.99,500,Ref,Px,Qt);
    free(pArt);
  }
  return 0;
}
