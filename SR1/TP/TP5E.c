#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>

#define MAX_PATH_LENGTH 1024
#define MAX_MAGIC_NUMBER_LENGTH 1024
#define MAGIC_NUMBER_LENGTH 5
#define N 4

typedef struct {
    char nom_fichier[MAX_PATH_LENGTH];
    int num_inode;
    off_t taille;
    char magic_number[MAGIC_NUMBER_LENGTH];
} ReponseFichier;

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

void executerCommande(const char *commande, const char *fichier) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        execlp(commande, commande, fichier, NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4 && argc != 5) {
        fprintf(stderr, "Usage: %s -v cmd magic_number repertoire\n", argv[0]);
        fprintf(stderr, "       %s -b magic_number_hex repertoire\n", argv[0]);
        exit(1);
    }

    const char *commande;
    char magic_number_hex[MAX_MAGIC_NUMBER_LENGTH];

    if (strcmp(argv[1], "-v") == 0) {
        commande = argv[2];
        const char *magic_number_str = argv[3];
        convertirMagicNumber(magic_number_str, magic_number_hex);
    } else if (strcmp(argv[1], "-b") == 0) { // Option -b
        commande = NULL;
        const char *magic_number_str = argv[2];
        strncpy(magic_number_hex, magic_number_str, MAX_MAGIC_NUMBER_LENGTH - 1);
    } else {
        fprintf(stderr, "Option invalide.\n");
        exit(2);
    }

    const char *repertoire = argv[argc - 1];

    int tube_traiter[2];
    int tube_reponse[2];
    if (pipe(tube_traiter) == -1 || pipe(tube_reponse) == -1) {
        perror("pipe");
        exit(3);
    }

    for (int i = 0; i < N; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(2);
        }

        if (pid == 0) {
            close(tube_traiter[1]);
            close(tube_reponse[0]);
            char nom_fichier[MAX_PATH_LENGTH];
            while (read(tube_traiter[0], nom_fichier, sizeof(nom_fichier))>0) {
                if (bytes_read == -1) {
                    perror("read");
                    exit(4);}

                int fd = open(nom_fichier, O_RDONLY);
                if (fd == -1) {
                    perror("open");
                    exit(5);
                }
                char magic_number_check[MAGIC_NUMBER_LENGTH];
                if (read(fd, magic_number_check, MAGIC_NUMBER_LENGTH) != MAGIC_NUMBER_LENGTH) {
                    perror("read");
                    exit(6);
                }
                close(fd);
                if (strncmp(magic_number_check, magic_number_hex, MAGIC_NUMBER_LENGTH) == 0) 
                    ReponseFichier reponse;
                    strncpy(reponse.nom_fichier, nom_fichier, sizeof(reponse.nom_fichier));
                    struct stat st;
                    if (stat(nom_fichier, &st) == -1) {
                        perror("stat");
                        exit(7);
                    }
                    reponse.num_inode = st.st_ino;
                    reponse.taille = st.st_size;
                    strncpy(reponse.magic_number, magic_number_hex, MAGIC_NUMBER_LENGTH);
                    write(tube_reponse[1], &reponse, sizeof(reponse));
                }
            }
        exit(0);
        }
    }

    // Création du processus fils traiterReponse
    pid_t pid_reponse = fork();
    if (pid_reponse == -1) {
        perror("fork");
        exit(1);
    }
    
    if (pid_reponse == 0) {
        close(tube_reponse[1]);
        close(tube_traiter[0]);
        close(tube_traiter[1]);
        ReponseFichier reponse;
        while (ssize_t bytes_read = read(tube_reponse[0], &reponse, sizeof(reponse))>0) {
            if (bytes_read == -1) {
                perror("read");
                exit(2);
            }
        printf("[%s] : Inode %d, Taille %lld, Magic Number %s\n", reponse.nom_fichier, reponse.num_inode, (long long)reponse.taille, reponse.magic_number);
        if (commande != NULL) {
            executerCommande(commande, reponse.nom_fichier);
        }
    }
    DIR *dir;
    struct dirent *entry;
    if ((dir = opendir(repertoire)) != NULL) {
    while ((entry = readdir(dir)) != NULL) {
        char chemin[MAX_PATH_LENGTH];
        snprintf(chemin, sizeof(chemin), "%s/%s", repertoire, entry->d_name);
        if (access(chemin, F_OK) == 0) {
            if (entry->d_type == DT_REG) {
                write(tube_traiter[1], chemin, strlen(chemin) + 1);
            }
        }
    }
    closedir(dir);
    } else {
    perror("opendir");
    exit(3);
    }
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
