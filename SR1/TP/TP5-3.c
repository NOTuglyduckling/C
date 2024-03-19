#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>

#define MAX_PATH_LENGTH 4096
#define MAX_MAGIC_NUMBER_LENGTH 1024
#define MAGIC_NUMBER_LENGTH 5
#define N 4


typedef struct {
    char nom_fichier[MAX_PATH_LENGTH];
    int num_inode;
    off_t taille;
    char magic_number[MAGIC_NUMBER_LENGTH];
} ReponseFichier;

void parcourir_sous_arborescence(const char *repertoire, int *tube_traiter) {
    DIR *dir;
    struct dirent *entree;
    struct stat infos;

    if ((dir = opendir(repertoire)) == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entree = readdir(dir)) != NULL) {
        char chemin[1024];
        snprintf(chemin, sizeof(chemin), "%s/%s", repertoire, entree->d_name);

        if (stat(chemin, &infos) == -1) {
            perror("stat");
            exit(EXIT_FAILURE);
        }
        if (S_ISDIR(infos.st_mode)) {
            if (strcmp(entree->d_name, ".") != 0 && strcmp(entree->d_name, "..") != 0) {
                parcourir_sous_arborescence(chemin,tube_traiter);}
        } else if (S_ISREG(infos.st_mode)) {
            if (infos.st_size == 0) {
                if (write(tube_traiter[1], &chemin, sizeof(chemin))==-1){
                    perro("write");
                    exit(1);
                }
            }
        }
    }
    closedir(dir);
}

void convertirMagicNumber(const char *magic_number_str, char *magic_number_hex) {
    char *token = strtok(magic_number_str, " ");
    while (token != NULL) {
        // Convertir chaque token en valeur hexadécimale et l'ajouter à la chaîne résultante
        int value;
        sscanf(token, "%x", &value);
        sprintf(magic_number_hex, "%02X", value);
        magic_number_hex += 2;
        token = strtok(NULL, " ");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4 && argc != 5 && argc!= 3) {
        fprintf(stderr, "Usage: %s -v cmd magic_number repertoire\n", argv[0]);
        fprintf(stderr, "       %s -b magic_number_hex repertoire\n", argv[0]);
        fprintf(stderr, "       %s magic_number repertoire\n", argv[0]);
        exit(1);
    }

    const char *commande;
    char magic_number_hex[MAX_MAGIC_NUMBER_LENGTH];
    const char *repertoire = argv[argc - 1];
    const char *magic_number_str;

    if (strcmp(argc == 5 && argv[1], "-v") == 0) {
        commande = argv[2];
        magic_number_str = argv[3];
        convertirMagicNumber(magic_number_str, magic_number_hex);
    } else if (argc == 4 && strcmp(argv[1], "-b") == 0) {
        commande = NULL;
        magic_number_str = argv[2];
        strncpy(magic_number_hex, magic_number_str, MAX_MAGIC_NUMBER_LENGTH - 1);
    } else if (argc!=3){
        fprintf(stderr, "Option invalide.\n");
        exit(1);
    }

    int tube_traiter[2];
    int tube_reponse[2];
    if (pipe(tube_traiter) == -1 || pipe(tube_reponse) == -1) {
        perror("pipe");
        exit(2);
    }

    for (int i = 0; i < N; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(3);}
        if (pid == 0) { // Code des fils traiterFichier
            close(tube_traiter[1]); 
            close(tube_reponse[0]);
            char nf[MAX_PATH_LENGTH];
            while (read(tube_traiter[0], &nf, sizeof(nf)) > 0){
                int fs = open(nf, O_RDONLY);
                fseek(fs,0,SEEK_SET);
                char magic_number[5];
                fread(magic_number,sizeof(magic_number),5,fs);
                if (strcmp(magic_number,magic)==0){
                    if (write(tube_reponse[1], &nf, sizeof(nf))==-1){
                    perro("write");
                    exit(1);
                }
                }
            }
            close(tube_traiter[0]);
            cloe(tube_reponse[1]);
            exit(0);
        }
    }
    pid_t pidr = fork();
    if (pidr == -1) {
        perror("fork");
        exit(4);}
    if (pidr == 0) {
        close(tube_reponse[1]); // Fermeture du descripteur d'écriture du tube dans le fils
        close(tube_traiter[0]); // Fermeture du descripteur de lecture du tube dans le fils
        
        exit(0);}

    // Processus père 
    parcourir_sous_arborescence(repertoire,tube_traiter);




    for (int i = 0; i < N; i++) {
        wait(NULL);
    }
    wait(NULL);
    close(tube_traiter[0]);
    close(tube_traiter[1]);
    close(tube_reponse[0]);
    close(tube_reponse[1]);
    return 0;
}