#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "tabrout.h"

#define BUF_SIZE 64
#define IPV4_ADR_STRLEN 16
#define LOCALHOST "127.0.0.1"
#define NO_BASE_PORT 17900

// Structure pour passer les arguments aux threads
typedef struct {
    char my_ip[IPV4_ADR_STRLEN];
    int my_number;
    int neighbor_number;
    routing_table_t *routing_table;
} thread_args_t;

// Fonction pour le rôle de récepteur
void *role_recepteur(void *args) {
    thread_args_t *t_args = (thread_args_t *)args;
    int port = NO_BASE_PORT + t_args->my_number;
    char msg_recu[BUF_SIZE];
    struct sockaddr_in serverSockAddr;
    int to_server_socket = -1;

    /* Création de la structure du socket */
    memset(&serverSockAddr, 0, sizeof(serverSockAddr));
    serverSockAddr.sin_port = htons(port);
    serverSockAddr.sin_family = AF_INET;
    serverSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Création de la socket */
    if ((to_server_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Erreur lors de la création de la socket (récepteur)");
        pthread_exit(NULL);
    }

    /* Bind (seulement chez le récepteur) */
    if (bind(to_server_socket, (struct sockaddr *)&serverSockAddr, sizeof(serverSockAddr)) == -1) {
        perror("Erreur lors du bind (récepteur)");
        close(to_server_socket);
        pthread_exit(NULL);
    }

    printf("Récepteur : En attente de messages sur le port %d...\n", port);

    // Réception du nombre d'entrées
    char nb_entry[4];
    if (recvfrom(to_server_socket, nb_entry, sizeof(nb_entry), 0, NULL, NULL) == -1) {
        perror("Erreur lors de la réception du nombre d'entrées");
        close(to_server_socket);
        pthread_exit(NULL);
    }

    printf("Récepteur : Nombre d'annonces reçues : %s\n", nb_entry);

    // Réception des annonces
    for (int i = 0; i < atoi(nb_entry); i++) {
        if (recvfrom(to_server_socket, msg_recu, sizeof(msg_recu), 0, NULL, NULL) == -1) {
            perror("Erreur lors de la réception d'une annonce");
            close(to_server_socket);
            pthread_exit(NULL);
        }
        printf("Récepteur : Annonce reçue : %s\n", msg_recu);
        add_entry_routing_table(t_args->routing_table, msg_recu);
    }

    printf("Récepteur : Mise à jour de la table de routage :\n");
    display_routing_table(t_args->routing_table, "Récepteur");

    close(to_server_socket);
    pthread_exit(NULL);
}

// Fonction pour le rôle d'émetteur
void *role_emetteur(void *args) {
    thread_args_t *t_args = (thread_args_t *)args;
    int port = NO_BASE_PORT + t_args->neighbor_number;
    struct sockaddr_in serverSockAddr;
    int to_server_socket = -1;

    /* Création de la structure du socket */
    memset(&serverSockAddr, 0, sizeof(serverSockAddr));
    serverSockAddr.sin_port = htons(port);
    serverSockAddr.sin_family = AF_INET;
    serverSockAddr.sin_addr.s_addr = inet_addr(LOCALHOST);

    /* Création de la socket */
    if ((to_server_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Erreur lors de la création de la socket (émetteur)");
        pthread_exit(NULL);
    }

    printf("Émetteur : Envoi des annonces au voisin sur le port %d...\n", port);

    // Envoi du nombre d'entrées
    char nb_entry[4];
    sprintf(nb_entry, "%d", t_args->routing_table->nb_entry);
    if (sendto(to_server_socket, nb_entry, strlen(nb_entry) + 1, 0, (struct sockaddr *)&serverSockAddr, sizeof(serverSockAddr)) < 0) {
        perror("Erreur lors de l'envoi du nombre d'entrées");
        close(to_server_socket);
        pthread_exit(NULL);
    }

    // Envoi des annonces
    for (int i = 0; i < t_args->routing_table->nb_entry; i++) {
        if (sendto(to_server_socket, t_args->routing_table->tab_entry[i], strlen(t_args->routing_table->tab_entry[i]) + 1, 0,
                   (struct sockaddr *)&serverSockAddr, sizeof(serverSockAddr)) < 0) {
            perror("Erreur lors de l'envoi d'une annonce");
            close(to_server_socket);
            pthread_exit(NULL);
        }
        printf("Émetteur : Annonce envoyée : %s\n", t_args->routing_table->tab_entry[i]);
    }

    printf("Émetteur : Envoi terminé.\n");

    close(to_server_socket);
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("Usage: %s IDIP@ssRouter MyNumberRouter NeighborNumberRouter\n", argv[0]);
        exit(1);
    }

    char myId[32];
    char idInitConfigFile[20];
    routing_table_t myRoutingTable;

    /* Construction des identifiants */
    sprintf(myId, "R%s %s", argv[2], argv[1]);
    sprintf(idInitConfigFile, "R%sCfg.txt", argv[2]);

    printf("ROUTEUR : %s\n", myId);

    /* Chargement de la table de routage */
    init_routing_table(&myRoutingTable, idInitConfigFile);
    printf("ROUTEUR : %d entrées initialement chargées\n", myRoutingTable.nb_entry);
    display_routing_table(&myRoutingTable, myId);

    // Préparation des arguments pour les threads
    thread_args_t t_args;
    strcpy(t_args.my_ip, argv[1]);
    t_args.my_number = atoi(argv[2]);
    t_args.neighbor_number = atoi(argv[3]);
    t_args.routing_table = &myRoutingTable;

    // Création des threads
    pthread_t thread_recepteur, thread_emetteur;

    pthread_create(&thread_recepteur, NULL, role_recepteur, &t_args);
    pthread_create(&thread_emetteur, NULL, role_emetteur, &t_args);

    // Attente des threads
    pthread_join(thread_recepteur, NULL);
    pthread_join(thread_emetteur, NULL);

    printf("ROUTEUR : Processus terminé.\n");
    return 0;
}
