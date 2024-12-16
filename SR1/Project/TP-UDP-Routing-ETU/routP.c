#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <wait.h>
#include "tabrout.h"

#define BUF_SIZE 64
#define IPV4_ADR_STRLEN 16
#define LOCALHOST "127.0.0.1"
#define NO_BASE_PORT 17900

/* Processus Récepteur */
void receiver(int myNumber, routing_table_t *routingTable) {
    int sock;
    struct sockaddr_in myAddr, senderAddr;
    char buffer[BUF_SIZE];
    socklen_t addrLen = sizeof(senderAddr);

    // Creation socket UDP
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Erreur création socket Récepteur");
        exit(EXIT_FAILURE);
    }

    // Bind socket au port de se routeur
    memset(&myAddr, 0, sizeof(myAddr));
    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons(NO_BASE_PORT + myNumber);
    myAddr.sin_addr.s_addr = inet_addr(LOCALHOST);

    if (bind(sock, (struct sockaddr *)&myAddr, sizeof(myAddr)) < 0) {
        perror("Erreur bind Récepteur");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Recepteur: Ecoute pour recevoir table de routage...\n");

    // Recevoir nombre d'entrées table de routage
    int received = recvfrom(sock, buffer, BUF_SIZE - 1, 0, (struct sockaddr *)&senderAddr, &addrLen);
    if (received < 0) {
        perror("Erreur réception nombre d'entrées");
        close(sock);
        exit(EXIT_FAILURE);
    }
    buffer[received] = '\0';
    int numEntries = atoi(buffer);

    // Recevoir entrées table de routage
    for (int i = 0; i < numEntries; i++) {
        received = recvfrom(sock, buffer, BUF_SIZE - 1, 0, (struct sockaddr *)&senderAddr, &addrLen);
        if (received < 0) {
            perror("Erreur Réception entrée table de routage");
            continue;
        }
        buffer[received] = '\0';

        if (!is_present_entry_table(routingTable, buffer)) {
            add_entry_routing_table(routingTable, buffer);
        }
    }

    printf("Récepteur : Table de routage mise à jour :\n");
    display_routing_table(routingTable, "Récepteur");

    close(sock);
}

/* Processus Emetteur */
void emitter(int neighborNumber, routing_table_t *routingTable) {
    int sock;
    struct sockaddr_in neighborAddr;
    char buffer[BUF_SIZE];

    // Creation socket UDP
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Erreur création socket Emetteur");
        exit(EXIT_FAILURE);
    }

    // Definir l'adresse du routeur voisin
    memset(&neighborAddr, 0, sizeof(neighborAddr));
    neighborAddr.sin_family = AF_INET;
    neighborAddr.sin_port = htons(NO_BASE_PORT + neighborNumber);
    neighborAddr.sin_addr.s_addr = inet_addr(LOCALHOST);

    printf("Emetteur: Envoi de mises à jour au voisin...\n");

    // Envoi nombre d'entrées
    snprintf(buffer, sizeof(buffer), "%d", routingTable->nb_entry);
    if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&neighborAddr, sizeof(neighborAddr)) < 0) {
        perror("Erreur envoi nombre d'entrées");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Envoi entrées table de routage
    for (int i = 0; i < routingTable->nb_entry; i++) {
        if (sendto(sock, routingTable->tab_entry[i], strlen(routingTable->tab_entry[i]), 0,
                   (struct sockaddr *)&neighborAddr, sizeof(neighborAddr)) < 0) {
            perror("Erreur envoi entrée table de routage");
        }
    }

    printf("Emetteur: Mises à jour table de routage envoyées.\n");
    close(sock);
}

/* Fonction Principale */
int main(int argc, char **argv) {
    if (argc != 4) {
        printf("Usage: %s MyIPAdresseID MyNumber NeighborNumber\n", argv[0]);
        exit(1);
    }

    char idInitConfigFile[20];
    char myId[32];
    routing_table_t myRoutingTable;

    // Construction Router ID et Config File ID
    sprintf(myId, "R%s %s", argv[2], argv[1]);
    sprintf(idInitConfigFile, "R%sCfg.txt", argv[2]);

    // Charge table de routage
    init_routing_table(&myRoutingTable, idInitConfigFile);
    printf("Router %s: Table de routage Initiale :\n", myId);
    display_routing_table(&myRoutingTable, myId);
    printf("\n");

    // Création fils pour recepteur
    pid_t pid = fork();
    if (pid < 0) {
        perror("Erreur Fork");
        exit(2);
    }

    if (pid == 0) {
        // Processus fils Récepteur
        receiver(atoi(argv[2]), &myRoutingTable);
        exit(0);
    } else {
        // Processus père Emetteur
        emitter(atoi(argv[3]), &myRoutingTable);
        // Attendre fin fils
        wait(NULL);
    }
    return 0;
}
