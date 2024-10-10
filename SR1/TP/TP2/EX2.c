#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h> 

void Nieme(int n, int entier, const char *fichier) {
    int file = open(fichier,O_RDWR|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
    if (file == -1) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(1);
    }

    if (lseek(file, (n - 1) * sizeof(int), SEEK_SET) == -1) {
        perror("Erreur lors du positionnement dans le fichier");
        close(file);
        exit(2);
    }

    if (entier == -1) {
        int value;
        if (read(file, &value,sizeof(int) ) == -1) {
            perror("Erreur lors de la lecture du fichier");
            close(file);
            exit(3);
        }
        printf("Nième entier : %d\n", value);
    } else {
        if (write(file, &entier, sizeof(entier)) == -1) {
            perror("Erreur lors de l'écriture dans le fichier");
            close(file);
            exit(4);
        }
        printf("Nième entier modifié avec succès\n");
    }

    if(close(file)==-1){
        perror("CLose FILE");
        exit(3);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4 && argc != 3) {
        fprintf(stderr, "Usage: %s n [entier] fichier\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Erreur : n doit être un entier positif\n");
        return 2;
    }const char *fichier = argv[3];

    
    int entier = -1;
    entier = atoi(argv[2]);

    Nieme(n, entier, fichier);

    return 0;
}