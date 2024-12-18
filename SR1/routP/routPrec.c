/*================================================*/
//Programme simulant un protocole de routage dynamique simplifié
// Ce programme code uniquement le comportement
// de récpeteur d'une annonce de routage
// émise depuis UN SEUL routeur voisin pour UN échange initial de routes
// T. Desprats - Novembre 2022

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h> // struct sockaddr_in
#include <time.h>
#include <signal.h>
#include <stdbool.h>

#include "tabrout.h"

#define BUF_SIZE_IN 64 // we should receive less...
#define IPV4_ADR_STRLEN 16  // == INET_ADDRSTRLEN
#define LOCALHOST "127.0.0.1"
#define NO_BASE_PORT 17900  // base number for computing real port number


/* =================================================================== */
/* FONCTION PRINCIPALE : PEER PROCESSUS DE ROUTAGE ROLE RECEPTEUR ONLY */
/* =================================================================== */
int main(int argc, char **argv) {

  // Usage routPrec IDIP@ssRouter  MyNumberRouter NeigborNumberRouter
  // Example routPrec 10.1.1.1 1 2

  char idInitConfigFile [20]; //Id of the configuration file of the router
  char myId [32]; // String array representing the whole id of the Router

  routing_table_t myRoutingTable; //Routing TABLE



  /* Building ID Router from command args */
  sprintf(myId,"R%s %s",argv[2],argv[1]);
  printf("ROUTEUR : %s\n",myId );
  //printf("construction id fichier\n");
  /* Building Config File ID from command args */
  sprintf(idInitConfigFile,"R%sCfg",argv[2]);
  strcat(idInitConfigFile,".txt");
  //printf("\n Nom fichier Configuration : %s",idInitConfigFile);
  /* Loading My Routing Table from Initial Config file */
  init_routing_table(&myRoutingTable, idInitConfigFile);
  printf("ROUTEUR : %d entrées initialement chargées \n",myRoutingTable.nb_entry);
  display_routing_table(&myRoutingTable,myId);

  /* A COMPLETER PAR LES ETUDIANTS ...
  ************************************/
  // Variables :
  int port = NO_BASE_PORT + atoi(argv[2]);
  char msg_recu[BUF_SIZE_IN];
  struct sockaddr_in serverSockAddr;
  int to_server_socket = -1;

  /* Création de la structure du socket */
  memset(&serverSockAddr, 0, sizeof(serverSockAddr));
  serverSockAddr.sin_port = htons(port);
  serverSockAddr.sin_family = AF_INET;
  serverSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  /* Création de la socket */
  if ((to_server_socket = socket(AF_INET,SOCK_DGRAM,0)) < 0) {
      printf("Probleme de la creation socket client\n");
      exit(0);
  }

  /* Bind (seulement chez le réceupteur) */
  if (bind(to_server_socket, (struct sockaddr *) &serverSockAddr, sizeof(serverSockAddr)) == -1) {
      printf("Probleme de bind\n");
      close(to_server_socket);
      exit(0);
  }

  char nb_entry[4];
  if (recvfrom(to_server_socket, nb_entry, 64, 0, NULL, NULL) == -1) {
      printf("Problème lors de la réception du nb_entry\n");
      close(to_server_socket);
      exit(0);
  }
  for (int i = 0; i < atoi(nb_entry); i++) {
      if (recvfrom(to_server_socket, &msg_recu, sizeof(msg_recu), 0, NULL, NULL) == -1) {
          printf("Problème lors de la réception d'une tab_entry\n");
          close(to_server_socket);
          exit(0);
      }
      add_entry_routing_table(&myRoutingTable, msg_recu);
  }

  // Display new content of my routing table
  display_routing_table(&myRoutingTable,myId);

  /* Fermeture de la socket */
  close(to_server_socket);
  exit(EXIT_SUCCESS);
 }
