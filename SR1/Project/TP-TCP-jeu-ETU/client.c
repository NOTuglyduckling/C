/* =================================================================== */
// Progrmame Client à destination d'un joueur qui doit deviner la case
// du trésor. Après chaque coup le résultat retourné par le serveur est
// affiché. Le coup consite en une abcsisse et une ordonnée (x, y).
/* =================================================================== */

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

//SERVEURNAME "146.59.237.169"

/* ====================================================================== */
/*                  Affichage du jeu en mode texte brut                   */
/* ====================================================================== */
void afficher_jeu(int jeu[N][N], int res, int points, int coups) {

    printf("\n************ TROUVEZ LE TRESOR ! ************\n");
    printf("    ");
    for (int i=0; i<10; i++)
        printf("  %d ", i+1);
    printf("\n    -----------------------------------------\n");
    for (int i=0; i<10; i++){
        printf("%2d  ", i+1);
        for (int j=0; j<10; j++) {
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
    struct hostent *serverHostEnt;
    long hostAddr;
    long status;
    char buffer[512];
    int sock_id, server_port;
    char server_ip[16];

    /* Init args */
    if (argc!=3) {
        printf("Usage: %s adresse_ip port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    strcpy(server_ip, argv[1]);
    server_port = atoi(argv[2]);

    /* Init jeu */
    for (int i=0; i<N; i++)
        for (int j=0; j<N; j++)
            jeu[i][j] = -1;

    /* Creation socket TCP  GOOOOOOODDDDDDD*/
    int to_server_socket=-1;
    if ( (to_server_socket = socket(AF_INET,SOCK_STREAM,0)) < 0){
        printf("Probleme de la creation socket client\n");
        exit(0);
    }
    

    /* Init caracteristiques serveur distant (struct sockaddr_in) */
    memset(&serverSockAddr, 0, sizeof(serverSockAddr));
    serverSockAddr.sin_family = AF_INET;
    serverSockAddr.sin_port = htons(server_port); /* host to network byte order */
    serverSockAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* any interface */

    /* Etablissement connexion TCP avec process serveur distant */
    if(connect( to_server_socket,(struct sockaddr *)&serverSockAddr,sizeof(serverSockAddr)) < 0 ){
        printf("Probleme de la demande de connection\n");
        exit(0);
    }

    /* Tentatives du joueur : stoppe quand tresor trouvé */
    do {
        afficher_jeu(jeu, res, points, coups);
        printf("\nEntrer le numéro de ligne : ");
        scanf("%d", &lig);
        printf("Entrer le numéro de colonne : ");
        scanf("%d", &col);

        /* Construction requête (serialisation en chaines de caractères) */
        // TODO

        /* Envoi de la requête au serveur (send) */
        // TODO

        /* Réception du resultat du coup (recv) */
        // TODO

        /* Deserialisation du résultat en un entier */
        // TODO
        res = 0;

        /* Mise à jour */
        if (lig>=1 && lig<=N && col>=1 && col<=N)
            jeu[lig-1][col-1] = res;
        points += res;
        coups++;

    } while (res);

    /* Fermeture connexion TCP */
    shutdown(to_server_socket,2);
    close(to_server_socket);

    /* Terminaison du jeu : le joueur a trouvé le tresor */
    afficher_jeu(jeu, res, points, coups);
    printf("\nBRAVO : trésor trouvé en %d essai(s) avec %d point(s)"
            " au total !\n\n", coups, points);
    return 0;
}
