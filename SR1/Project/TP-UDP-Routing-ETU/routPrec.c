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
  if (argc != 4) {
    printf("Usage: %s IDIP@ssRouter MyNumberRouter NeighborNumberRouter\n", argv[0]);
    exit(1);
  }
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

  // Creation socket UDP
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) {
    perror("Erreur création socket");
    exit(2);
  }

  // Bind socket pour recevoir
  struct sockaddr_in myAddr;
  memset(&myAddr, 0, sizeof(myAddr));
  myAddr.sin_family = AF_INET;
  myAddr.sin_port = htons(NO_BASE_PORT + atoi(argv[2]));
  myAddr.sin_addr.s_addr = inet_addr(LOCALHOST);

  if (bind(sock, (struct sockaddr *)&myAddr, sizeof(myAddr)) < 0) {
      perror("Erreur Bind");
      close(sock);
      exit(3);
  }

  // Recevoir nombre d'entrées
  char buffer[BUF_SIZE_IN];
  int received = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, NULL, NULL);
  if (received < 0) {
      perror("Erreur réception nombre d'entrées");
      close(sock);
      exit(4);
  }
  buffer[received] = '\0';
  int numEntries = atoi(buffer);

  // Recevoir entrées table de routage
  for (int i = 0; i < numEntries; i++) {
      received = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, NULL, NULL);
      if (received < 0) {
          perror("Erreur réception entrée table de routage");
          continue;
      }
      buffer[received] = '\0';

      // Ajouter l'entrée a notre table si elle n'est pas déjà présente
      if (!is_present_entry_table(&myRoutingTable, buffer)) {
          add_entry_routing_table(&myRoutingTable, buffer);
      }
  }

  // Afficher nouvelle table de routage
  display_routing_table(&myRoutingTable,myId);
  close(sock);
  exit(0);
}


    

    




