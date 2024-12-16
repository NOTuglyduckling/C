# TP Jeu TCP

Jeu de tresor utilisant la connection TCP entre le client et le serveur.

## Description

Jeu de trésor avec 2 serveur différents, le premier codé en itératif accepte un seul client à la fois et le deuxième codé en concurrent utilise des processus fils pour accepter plusieurs clients à la fois.

## Getting Started

### Dépandences

A lancer sous linux

### Installing

Avant toute première utilisation utilisez la commande make pour crée les exécutables.

### Executer le programme

* Ouvrir deux terminaux
* d'abord lancer le serveur avec la commande
```
./serveur1
```
ou
```
./serveur2
```
* puis lancez le client avec la commande
```
./client @adresse_ip @port
```
*Si vous avez bien rentrer les bonnes adresses la connection est établie et vous avez un message de confirmation côter serveur et le jeu apparait côter client.
Utilisez 127.0.0.1 et le port 5555.

## Help

Si après avoir lancer le serveur 2 et vous essayez de le relancer après l'avoir ctrl+C et cela ne marche pas il faut kill le processus du serveur.

## Auteur

SEARLE Oliver 22210680 