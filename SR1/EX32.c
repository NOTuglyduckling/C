#define _POSIX_C_SOURCE 200809L /* norme POSIX.1-2017 */
#include <stdio.h> /* printf et perror */
#include <stdlib.h> /* exit, EXIT_SUCCESS... */
#include <sys/types.h> /* pid_t */
#include <unistd.h> /* fork, getpid et sleep */
#define NBPROC 3 /* Nombre de processus fils */
pid_t lesPids[NBPROC];
void AfficheLesPids( pid_t tab[])
{
    int k;
    for (k=0; k<NBPROC; k++)
        printf("%6d ",tab[k]);
    printf("\n");
}   
void maFonction ( int monNum)
{
    printf("[Fils] %d: contenu de lesPids = ", monNum);
    AfficheLesPids( lesPids);
}
int main(void){
    int i; /* Numéros des fils */
    printf("[Père] début: contenu de lesPids = ");
    AfficheLesPids( lesPids);
    for (i=0; i<NBPROC; i++){
        switch (lesPids[i]=fork()){
        case -1 : /* Problème */
            perror("Échec création du fils : ");
            exit(EXIT_FAILURE);
        case 0 : /* Je suis le fils */
            maFonction( i);
            exit(EXIT_SUCCESS);
        default : /* Je suis le père */
            break;
        }
    }
    printf("[Père] fin : contenu de lesPids = ");
    AfficheLesPids( lesPids);
    exit(EXIT_SUCCESS);
}