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

/* Receiver Process */
void receiver(int myNumber, routing_table_t *routingTable) {
    int sock;
    struct sockaddr_in myAddr, senderAddr;
    char buffer[BUF_SIZE];
    socklen_t addrLen = sizeof(senderAddr);

    // Create UDP socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Receiver socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to this router's port
    memset(&myAddr, 0, sizeof(myAddr));
    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons(NO_BASE_PORT + myNumber);
    myAddr.sin_addr.s_addr = inet_addr(LOCALHOST);

    if (bind(sock, (struct sockaddr *)&myAddr, sizeof(myAddr)) < 0) {
        perror("Receiver bind failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Receiver: Listening for routing updates...\n");

    // Receive number of entries
    int received = recvfrom(sock, buffer, BUF_SIZE - 1, 0, (struct sockaddr *)&senderAddr, &addrLen);
    if (received < 0) {
        perror("Failed to receive number of entries");
        close(sock);
        exit(EXIT_FAILURE);
    }
    buffer[received] = '\0';
    int numEntries = atoi(buffer);

    // Receive routing table entries
    for (int i = 0; i < numEntries; i++) {
        received = recvfrom(sock, buffer, BUF_SIZE - 1, 0, (struct sockaddr *)&senderAddr, &addrLen);
        if (received < 0) {
            perror("Failed to receive routing table entry");
            continue;
        }
        buffer[received] = '\0';

        if (!is_present_entry_table(routingTable, buffer)) {
            add_entry_routing_table(routingTable, buffer);
        }
    }

    printf("Receiver: Updated routing table:\n");
    display_routing_table(routingTable, "Receiver");

    close(sock);
}

/* Emitter Process */
void emitter(int neighborNumber, routing_table_t *routingTable) {
    int sock;
    struct sockaddr_in neighborAddr;
    char buffer[BUF_SIZE];

    // Create UDP socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Emitter socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define neighbor router address
    memset(&neighborAddr, 0, sizeof(neighborAddr));
    neighborAddr.sin_family = AF_INET;
    neighborAddr.sin_port = htons(NO_BASE_PORT + neighborNumber);
    neighborAddr.sin_addr.s_addr = inet_addr(LOCALHOST);

    printf("Emitter: Sending routing updates to neighbor...\n");

    // Send number of entries
    snprintf(buffer, sizeof(buffer), "%d", routingTable->nb_entry);
    if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&neighborAddr, sizeof(neighborAddr)) < 0) {
        perror("Failed to send number of entries");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Send routing table entries
    for (int i = 0; i < routingTable->nb_entry; i++) {
        if (sendto(sock, routingTable->tab_entry[i], strlen(routingTable->tab_entry[i]), 0,
                   (struct sockaddr *)&neighborAddr, sizeof(neighborAddr)) < 0) {
            perror("Failed to send routing table entry");
        }
    }

    printf("Emitter: Routing table updates sent.\n");
    close(sock);
}

/* Main Function */
int main(int argc, char **argv) {
    if (argc != 4) {
        printf("Usage: %s MyIPAdresseID MyNumber NeighborNumber\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char idInitConfigFile[20];
    char myId[32];
    routing_table_t myRoutingTable;

    // Build Router ID and Config File ID
    sprintf(myId, "R%s %s", argv[2], argv[1]);
    sprintf(idInitConfigFile, "R%sCfg.txt", argv[2]);

    // Load Routing Table
    init_routing_table(&myRoutingTable, idInitConfigFile);
    printf("Router %s: Initial Routing Table:\n", myId);
    display_routing_table(&myRoutingTable, myId);

    // Fork for receiver process
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process (Receiver)
        receiver(atoi(argv[2]), &myRoutingTable);
        exit(EXIT_SUCCESS);
    } else {
        // Parent process (Emitter)
        emitter(atoi(argv[3]), &myRoutingTable);
        // Wait for child process (Receiver) to finish
        wait(NULL);
    }

    display_routing_table(&myRoutingTable, myId);

    return 0;
}
