#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_BASSIN 5

int main() {
    int Remplir[2];
    int Etat[2];

    if (pipe(Remplir) == -1 || pipe(Etat) == -1){
        perror("pipe");
        exit(1);}
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(2);}
    if (pid == 0) {
        close(Remplir[1]);
        close(Etat[0]);
        int bassin = 0,n;
        while (read(Remplir[0], &n, sizeof(int)) > 0){
            if (n > 0){
                bassin += n;
                while (bassin >= MAX_BASSIN){
                    write(Etat[1], &bassin, sizeof(int));
                    bassin = 0;}
                printf("*");
                fflush(stdout);
            } else if (entier < 0){
                bassin = 0;
                printf("\n");}
        }
        close(Remplir[0]);
        close(Etat[1]);
        exit(1);
    } else {
        close(Remplir[0]);
        close(Etat[1]);
        int f_flags; /* mode non bloquant pour le tube Etat dans le P`ere */
        f_flags = fcntl(Etat[0], F_GETFL); /* R´ecup´eration des flags */
        f_flags |= O_NONBLOCK; /* Positionnement du flag de non blocage */
        fcntl(Etat[0], F_SETFL, f_flags); /* Mis `a jour des flags */

        int N = 10;
        for (int i = 0; i < N; i++) {
            write(Remplir[1], &i, sizeof(int));
            sleep(1);
            int bassin;
            if (read(Etat[0], &bassin, sizeof(int)) > 0) {
                printf("\n");
                printf("[Père] : Le bassin est plein avec un niveau de %d.\n", bassin);
            }
        }
        close(Remplir[1]);
        int status;
        waitpid(pid, &status, 0);
        printf("[Père] : Mon fils %d est terminé avec le code %d.\n", pid, WEXITSTATUS(status));
    }
    return 0;
}
