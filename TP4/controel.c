#include <stdio.h>

int main(){
    double vals[100];
    int somme=0,i=0;
    printf("donnez les valeurs : ");
    scanf("%le",&vals[i]);
    while (vals[i]!=0){
        if (vals[i]<0){
            somme+=vals[i];
            }
            i++;
        scanf("%le",&vals[i]);
        }
    printf("La somme des negatifs est %d", somme);
    return 0;
}