#ifndef MATRICE_H
#define MATRICE_H

typedef unsigned char** tMatrice;
extern tMatrice MatAllouer(int pNbLig, int pNbCol);
extern tMatrice MatLire(int* pNbLig, int* pNbCol);
extern void MatAfficher(tMatrice Mat, int NbLIg, int NbCol);
extern tMatrice MatCopier(tMatrice Mat, int NbLig, int NbCol);
extern void MatLiberer(tMatrice* pMat);

#endif
