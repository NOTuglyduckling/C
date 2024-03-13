#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

void Affiche_inode(struct stat *Infos) {
    // Détermination du type de fichier
    char *type;
    if (S_ISREG(Infos->st_mode)) {
        type = "fichier ordinaire";
    } else if (S_ISDIR(Infos->st_mode)) {
        type = "répertoire";
    } else if (S_ISLNK(Infos->st_mode)) {
        type = "lien symbolique";
    } else if (S_ISBLK(Infos->st_mode)) {
        type = "périphérique bloc";
    } else if (S_ISCHR(Infos->st_mode)) {
        type = "périphérique caractère";
    } else if (S_ISFIFO(Infos->st_mode)) {
        type = "fifo (named pipe)";
    } else if (S_ISSOCK(Infos->st_mode)) {
        type = "socket";
    } else {
        type = "inconnu";
    }

    // Affichage des informations
    printf("Type: %s, Taille: %ld octets, Date de dernière modification: %s", 
            type, 
            (long)Infos->st_size,
            ctime(&Infos->st_mtime));
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <nom_fichier>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct stat Infos;
    if (lstat(argv[1], &Infos) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    printf("Fichier: %s\n", argv[1]);
    Affiche_inode(&Infos);

    return 0;
}
