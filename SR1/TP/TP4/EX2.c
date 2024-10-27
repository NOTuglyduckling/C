#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>

#define MAX_BASSIN 5

int main(){
    int Remplir[2],Etat[2];
    if ((pipe(Remplir)==-1)||(pipe(Etat)==-1)){
        perror("pipe");
        exit(1);
    }
    pid_t pid = fork();
    switch(pid){
        case -1:{
            perror("fork");
            exit(2);
        }
        case 0: {
            close(Remplir[1]);
            close(Etat[0]);
            int bassin=0,n;
            while (read(Remplir[0], &n, sizeof(int))>0){
                if (n>0){
                    bassin+=n;
                    if (bassin> MAX_BASSIN){
                        write(Etat[1],&bassin,sizeof(int));
                    } else {
                        printf("*");
                        fflush(stdout);}
                } else if (n<0){
                    bassin=0;
                    printf("\n");
                }
            }
            close(Remplir[0]);
            close(Etat[1]);
            printf("\n");
            exit(1);
        }
        default: {
            close(Remplir[0]);
            close(Etat[1]);
            int f_flags; /* mode non bloquant pour le tube Etat dans le P`ere */
            f_flags = fcntl(Etat[0], F_GETFL); /* R´ecup´eration des flags */
            f_flags |= O_NONBLOCK; /* Positionnement du flag de non blocage */
            fcntl(Etat[0], F_SETFL, f_flags); /* Mis `a jour des flags */

            int N=10,p=1,e=-1;
            for (int i =0; i<=N; i++){
                write(Remplir[1],&p,sizeof(int));
                sleep(1);
                int martin;
                while (read(Etat[0],&martin,sizeof(int))>0){
                    write(Remplir[1],&martin,sizeof(int));
                }
            }
            close(Remplir[1]);
            close(Etat[0]);
            int status;
            waitpid(pid,&status,0);
            printf("[Pere] : Mon fils %d est termine avec le code %d.",pid,WEXITSTATUS(status));
        }
    }    
    return 0;
}