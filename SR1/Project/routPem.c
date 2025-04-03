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

  // Usage routPem IDIP@ssRouter  MyNumberRouter NeigborNumberRouter
  // Example routPem 10.1.1.1 1 2

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

  /* A COMPLETER PAR LES ETUDIANTS ....
  ************************************** */
  /*  Variables */
  int port = NO_BASE_PORT + atoi(argv[3]);
  struct sockaddr_in serverSockAddr;
  int to_server_socket = -1;

  /* Création de la structure du socket */
  memset(&serverSockAddr, 0, sizeof(serverSockAddr));
  serverSockAddr.sin_port = htons(port);
  serverSockAddr.sin_family = AF_INET;
  serverSockAddr.sin_addr.s_addr = inet_addr(LOCALHOST);

  /* Création de la socket */
  if ((to_server_socket = socket(AF_INET,SOCK_DGRAM,0)) < 0) {
      printf("Probleme lors de la creation de la socket\n");
      exit(0);
  }

  /* Envoi des tables de routage */
  char nb_entry_ch[4];
  sprintf(nb_entry_ch, "%d", myRoutingTable.nb_entry);
  if (sendto(to_server_socket, nb_entry_ch, strlen(nb_entry_ch)+1, 0, (struct sockaddr*) &serverSockAddr, sizeof(serverSockAddr)) < 0) {
    printf("Probleme lors de l'envoi\n");
    exit(0);
  }
  printf("test");
  for (int i = 0; i < myRoutingTable.nb_entry; i++) {
    printf("Table : %s\n", myRoutingTable.tab_entry[i]);
    printf("Taille : %ld\n", sizeof(myRoutingTable.tab_entry[i]));
    sendto(to_server_socket, myRoutingTable.tab_entry[i], sizeof(myRoutingTable.tab_entry), 0, (struct sockaddr*) &serverSockAddr, sizeof(serverSockAddr));
  }

  printf("Envois terminé avec succès.\n");

  /* Fermeture de la socket */
  close(to_server_socket);
  exit(EXIT_SUCCESS);
 }
