#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define N 10
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define MAGENTA "\033[35m"

/* ====================================================================== */
/*                  Affichage du jeu en mode texte brut                   */
/* ====================================================================== */
void afficher_jeu(int jeu[N][N], int res, int points, int coups) {
    printf("\n************ TROUVEZ LE TRESOR ! ************\n");
    printf("    ");
    for (int i = 0; i < N; i++)
        printf("  %d ", i + 1);
    printf("\n    -----------------------------------------\n");
    for (int i = 0; i < N; i++) {
        printf("%2d  ", i + 1);
        for (int j = 0; j < N; j++) {
            printf("|");
            switch (jeu[i][j]) {
                case -1:
                    printf(" 0 ");
                    break;
                case 0:
                    printf(GREEN " T " RESET);
                    break;
                case 1:
                    printf(YELLOW " %d " RESET, jeu[i][j]);
                    break;
                case 2:
                    printf(RED " %d " RESET, jeu[i][j]);
                    break;
                case 3:
                    printf(MAGENTA " %d " RESET, jeu[i][j]);
                    break;
            }
        }
        printf("|\n");
    }
    printf("    -----------------------------------------\n");
    printf("Pts dernier coup %d | Pts total %d | Nb coups %d\n", res, points, coups);
}

/* ====================================================================== */
/*                    Fonction principale                                 */
/* ====================================================================== */
int main(int argc, char **argv) {
    int jeu[N][N];
    int lig, col;
    int res = -1, points = 0, coups = 0;

    struct sockaddr_in serverSockAddr;
    char buffer[512];
    int to_server_socket;
    char server_ip[16];
    int server_port;

    /* Init args */
    if (argc != 3) {
        printf("Usage: %s adresse_ip port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    strcpy(server_ip, argv[1]);
    server_port = atoi(argv[2]);

    /* Init jeu */
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            jeu[i][j] = -1;

    /* Creation socket TCP */
    if ((to_server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    /* Init server characteristics */
    memset(&serverSockAddr, 0, sizeof(serverSockAddr));
    serverSockAddr.sin_family = AF_INET;
    serverSockAddr.sin_port = htons(server_port);

    /* Convert server IP address */
    if (inet_pton(AF_INET, server_ip, &serverSockAddr.sin_addr) <= 0) {
        perror("Invalid server IP address");
        close(to_server_socket);
        exit(EXIT_FAILURE);
    }

    /* Establish TCP connection */
    if (connect(to_server_socket, (struct sockaddr *)&serverSockAddr, sizeof(serverSockAddr)) < 0) {
        perror("Connection to server failed");
        close(to_server_socket);
        exit(EXIT_FAILURE);
    }

    /* Game loop: stop when treasure is found */
    do {
        afficher_jeu(jeu, res, points, coups);

        printf("\nEntrer le numéro de ligne : ");
        scanf("%d", &lig);
        printf("Entrer le numéro de colonne : ");
        scanf("%d", &col);

        /* Construct request (serialize as a string) */
        snprintf(buffer, sizeof(buffer), "%d %d", lig, col);

        /* Send the request to the server */
        if (send(to_server_socket, buffer, strlen(buffer), 0) < 0) {
            perror("Failed to send request");
            close(to_server_socket);
            exit(EXIT_FAILURE);
        }

        /* Receive the result from the server */
        int received = recv(to_server_socket, buffer, sizeof(buffer) - 1, 0);
        if (received < 0) {
            perror("Failed to receive response");
            close(to_server_socket);
            exit(EXIT_FAILURE);
        }
        buffer[received] = '\0';

        /* Deserialize the result */
        sscanf(buffer, "%d", &res);

        /* Update game state */
        if (lig >= 1 && lig <= N && col >= 1 && col <= N)
            jeu[lig - 1][col - 1] = res;
        points += res;
        coups++;
    } while (res != 0);

    /* Close TCP connection */
    shutdown(to_server_socket, SHUT_RDWR);
    close(to_server_socket);

    /* End of game: treasure found */
    afficher_jeu(jeu, res, points, coups);
    printf("\nBRAVO : trésor trouvé en %d essai(s) avec %d point(s)"
           " au total !\n\n", coups, points);

    return 0;
}
