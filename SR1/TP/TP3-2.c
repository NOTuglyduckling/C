#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

void parcourir_sous_arborescence(const char *repertoire, int *compteur) {
    DIR *dir;
    struct dirent *entree;
    struct stat infos;

    if ((dir = opendir(repertoire)) == NULL) {
        perror("opendir");
        exit(1);
    }

    while ((entree = readdir(dir)) != NULL) {
        char chemin[1024];
        snprintf(chemin, sizeof(chemin), "%s/%s", repertoire, entree->d_name);

        if (stat(chemin, &infos) == -1) {
            perror("stat");
            exit(2);
        }

        if (S_ISDIR(infos.st_mode)) {
            if (strcmp(entree->d_name, ".") != 0 && strcmp(entree->d_name, "..") != 0) {
                parcourir_sous_arborescence(chemin, compteur);
            }
        } else if (S_ISREG(infos.st_mode)) {
            if (infos.st_size == 0) {
                printf("%s\n", chemin);
                (*compteur)++;
            }
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <repertoire>\n", argv[0]);
        exit(1);
    }

    int compteur = 0;
    parcourir_sous_arborescence(argv[1], &compteur);
    printf("%d fichier(s) vide(s) trouvé(s) !\n", compteur);

    return 0;
}
