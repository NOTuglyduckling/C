/* =================================================================== */
// Progrmame Serveur qui calcule le résultat d'un coup joué à partir
// des coordonnées reçues de la part d'un client "joueur".
// Version ITERATIVE : 1 seul client/joueur à la fois
/* =================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "tresor.h"
/* =================================================================== */
/* FONCTION PRINCIPALE : SERVEUR ITERATIF                              */
/* =================================================================== */


#define PORT 5555
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    int treasure_x = 4, treasure_y = 5; // Position du trésor par défaut
    int points;

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    
    // Bind socket au port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Ecouter pour les connections
    if (listen(server_fd, 1) == -1) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accepter une connection client
        if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len)) == -1) {
            perror("Accept failed");
            continue; 
            // Continue a accepter de nouvelles connections
        }

        printf("Connection established with client...\n");

        // Initialiser une nouvelle partie pour le client client
        int found_treasure = 0;
        while (!found_treasure) {
            // Receive client's guess
            int received = recv(client_fd, buffer, BUFFER_SIZE, 0);
            if (received <= 0) {
                perror("Client disconnected or error");
                break;
            }
            buffer[received] = '\0';

            // Départager l'essai en coordonnées
            int guess_x, guess_y;
            sscanf(buffer, "%d %d", &guess_x, &guess_y);

            // Calculer les points de cet essai
            points = recherche_tresor(8,guess_x, guess_y, treasure_x, treasure_y);
            sprintf(buffer, "%d", points);

            // Envoyer le résultat de l'essai au client
            send(client_fd, buffer, strlen(buffer), 0);

            // Verifier si le Client a trouver le trésor
            if (points == 0) {
                printf("Treasure found by client!\n");
                found_treasure = 1;
            }
        }
        close(client_fd);
        printf("Connection closed with client.\n");
    }
    close(server_fd);
    return 0;
}
