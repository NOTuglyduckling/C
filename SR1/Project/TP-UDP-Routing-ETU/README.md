# TP Routage UDP

Programmes émetteur/récepteur pour manipuler le routage et le protocole UDP.

## Description

Le projet inclu 3 programmes différents :
* routPrec -> récepteur
* routPem -> émetteur
* routP -> récepteur/émetteur

## Getting Started

### Dépandences

A lancer sous linux.

### Mise en place

* Avant toute première utilisation utilisez les commandes :
```
make tpEmRec
```
Pour les émetteurs et récepteurs simples. (routPrec,routPem)
```
make tpPeer
```
Pour la partie émetteur/récepteur combiné. (routP)


### Executer le programme

* Ouvrir trois terminaux pour tester chaque cas.
* d'abord lancer le récepteur simple avec la commande :
```
./routPRec 10.1.1.1 1 2
```
* Puis lancer l'émetteur avec
```
./routPem 10.2.2.2 2 1
```

* Sinon vous pouvez également lancer deux fois le programme combiné par example :
```
./routP 10.2.2.2 2 3
```
```
./routP 10.3.3.3 3 2
```

* Les trois commandes fonctionnent de la même manière avec en arguments :
Adresse Ip du noeud, n° du noeud, n° du noeud voisin.

## Auteur

SEARLE Oliver 22210680 