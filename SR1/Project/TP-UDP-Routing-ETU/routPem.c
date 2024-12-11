/*=============================================================*/
//Programme simulant un protocole de routage dynamique simplifié
// Ce programme code uniquement le comportement
// d'émetteur d'une annonce de routage
// vers UN SEUL routeur voisin pour UN échange initial de routes
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

#define BUF_SIZE_OUT 64 // we should send less...
#define IPV4_ADR_STRLEN 16  // == INET_ADDRSTRLEN
#define LOCALHOST "127.0.0.1"
#define NO_BASE_PORT 17900  // base number for computing real port number

/* =================================================================== */
/* FONCTION PRINCIPALE : PEER PROCESSUS DE ROUTAGE ROLE EMETTEUR ONLY  */
/* =================================================================== */

int main(int argc, char **argv) {
  if (argc != 4) {
    printf("Usage: %s IDIP@ssRouter MyNumberRouter NeighborNumberRouter\n", argv[0]);
    exit(1);
  }
  // Usage routPem IDIP@ssRouter  MyNumberRouter NeigborNumberRouter
  // Example routPem 10.1.1.1 1 2
  int sock;
  struct sockaddr_in neighborAddr;
  char buffer[BUF_SIZE_OUT];

  char idInitConfigFile [20]; //Id of the configuration file of the router
  char myId [32]; // String array representing the whole id of the Router
  routing_table_t myRoutingTable; //Routing TABLE

  /* Building ID Router from command args */
  sprintf(myId,"R%s %s",argv[2],argv[1]);
  printf("ROUTEUR : %s\n",myId );
  printf("construction id fichier\n");

  /* Building Config File ID from command args */
  sprintf(idInitConfigFile,"R%sCfg",argv[2]);
  strcat(idInitConfigFile,".txt");
  printf("\n Nom fichier Configuration : %s",idInitConfigFile);

  /* Loading My Routing Table from Initial Config file */
  init_routing_table(&myRoutingTable, idInitConfigFile);
  printf("ROUTEUR : %d entrées initialement chargées \n",myRoutingTable.nb_entry);
  display_routing_table(&myRoutingTable,myId);

  // Create UDP socket
  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
      perror("Emitter socket creation failed");
      exit(EXIT_FAILURE);
  }

  // Define neighbor router address
  memset(&neighborAddr, 0, sizeof(neighborAddr));
  neighborAddr.sin_family = AF_INET;
  neighborAddr.sin_port = htons(NO_BASE_PORT + argv[2]);
  neighborAddr.sin_addr.s_addr = inet_addr(LOCALHOST);

  printf("Emitter: Sending routing updates to neighbor...\n");

  // Send number of entries
  snprintf(buffer, sizeof(buffer), "%d", myRoutingTable.nb_entry);
  if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&neighborAddr, sizeof(neighborAddr)) < 0) {
      perror("Failed to send number of entries");
      close(sock);
      exit(EXIT_FAILURE);
  }

  // Send routing table entries
  for (int i = 0; i < myRoutingTable.nb_entry; i++) {
    if (sendto(sock, myRoutingTable.tab_entry[i], strlen(myRoutingTable.tab_entry[i]), 0,(struct sockaddr *)&neighborAddr, sizeof(neighborAddr)) < 0) {
      perror("Failed to send routing table entry");
    }
  }

  printf("Emitter: Routing table updates sent.\n");
  close(sock);

  exit(1);
}
