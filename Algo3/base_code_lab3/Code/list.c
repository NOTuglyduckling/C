/*-----------------------------------------------------------------*/
/*
 Licence Informatique - Structures de données
 Mathias Paulin (Mathias.Paulin@irit.fr)
 
 Implantation du TAD List vu en cours.
 */
/*-----------------------------------------------------------------*/
#include <stdio.h>
#
#include <stdlib.h>
#include <assert.h>

#include "list.h"

typedef struct s_LinkedElement {
	int value;
	struct s_LinkedElement* previous;
	struct s_LinkedElement* next;
} LinkedElement;

/* Use of a sentinel for implementing the list :
 The sentinel is a LinkedElement* whose next pointer refer always to the head of the list and previous pointer to the tail of the list
 */
struct s_List {
	LinkedElement* sentinel;
	int size;
};


/*-----------------------------------------------------------------*/

LList* list_create(void) {
    List* l = (List*) malloc(sizeof(List));
    LinkedElement* sentinel = (LinkedElement*) malloc(sizeof(LinkedElement));
    sentinel->next = sentinel;
    sentinel->previous = sentinel;
    l->sentinel = sentinel;
    l->size = 0;
    return l;
}


/*-----------------------------------------------------------------*/

List* list_push_back(List* l, int v) {
    LinkedElement* new_elem = (LinkedElement*) malloc(sizeof(LinkedElement));
    new_elem->value = v;
    new_elem->next = l->sentinel;
    new_elem->previous = l->sentinel->previous;
    l->sentinel->previous->next = new_elem;
    l->sentinel->previous = new_elem;
    l->size++;
    return l;
}


/*-----------------------------------------------------------------*/

void list_delete(ptrList* l) {
    LinkedElement* current = (*l)->sentinel->next;
    while (current != (*l)->sentinel) {
        LinkedElement* next = current->next;
        free(current);
        current = next;
    }
    free((*l)->sentinel);
    free(*l);
    *l = NULL;
}


/*-----------------------------------------------------------------*/

List* list_push_front(List* l, int v) {
    LinkedElement* new_elem = (LinkedElement*) malloc(sizeof(LinkedElement));
    new_elem->value = v;
    new_elem->next = l->sentinel->next;
    new_elem->previous = l->sentinel;
    l->sentinel->next->previous = new_elem;
    l->sentinel->next = new_elem;
    l->size++;
    return l;
}


/*-----------------------------------------------------------------*/

int list_front(const List* l) {
    if (list_is_empty(l)) {
        // Gérer l'erreur (par exemple, retourner une valeur par défaut ou générer une erreur)
    }
    return l->sentinel->next->value;
}


/*-----------------------------------------------------------------*/

int list_back(const List* l) {
    if (list_is_empty(l)) {
        // Gérer l'erreur
    }
    return l->sentinel->previous->value;
}


/*-----------------------------------------------------------------*/

List* list_pop_front(List* l) {
    if (!list_is_empty(l)) {
        LinkedElement* first = l->sentinel->next;
        l->sentinel->next = first->next;
        first->next->previous = l->sentinel;
        free(first);
        l->size--;
    }
    return l;
}


/*-----------------------------------------------------------------*/

List* list_pop_back(List* l) {
    if (!list_is_empty(l)) {
        LinkedElement* last = l->sentinel->previous;
        last->previous->next = l->sentinel;
        l->sentinel->previous = last->previous;
        free(last);
        l->size--;
    }
    return l;
}


/*-----------------------------------------------------------------*/

List* list_insert_at(List* l, int p, int v) {
    if (p > l->size || p < 0) {
        // Gérer l'erreur
    }
    LinkedElement* new_elem = (LinkedElement*) malloc(sizeof(LinkedElement));
    new_elem->value = v;
    
    LinkedElement* current = l->sentinel->next;
    for (int i = 0; i < p; i++) {
        current = current->next;
    }
    new_elem->next = current;
    new_elem->previous = current->previous;
    current->previous->next = new_elem;
    current->previous = new_elem;
    l->size++;
    return l;
}


/*-----------------------------------------------------------------*/

List* list_remove_at(List* l, int p) {
    if (p >= l->size || p < 0) {
        // Gérer l'erreur
    }
    LinkedElement* current = l->sentinel->next;
    for (int i = 0; i < p; i++) {
        current = current->next;
    }
    current->previous->next = current->next;
    current->next->previous = current->previous;
    free(current);
    l->size--;
    return l;
}


/*-----------------------------------------------------------------*/

int list_at(const List* l, int p) {
    if (p >= l->size || p < 0) {
        // Gérer l'erreur
    }
    LinkedElement* current = l->sentinel->next;
    for (int i = 0; i < p; i++) {
        current = current->next;
    }
    return current->value;
}


/*-----------------------------------------------------------------*/

bool list_is_empty(const List* l) {
    return l->size == 0;
}


/*-----------------------------------------------------------------*/

int list_size(const List* l) {
    return l->size;
}


/*-----------------------------------------------------------------*/

List* list_map(List* l, Functor f, void* environment) {
    LinkedElement* current = l->sentinel->next;
    while (current != l->sentinel) {
        current->value = f(current->value, environment);
        current = current->next;
    }
    return l;
}


/*-----------------------------------------------------------------*/

List* list_sort(List* l, OrderFunctor f) {
    if (list_is_empty(l)) {
        return l;
    }

    // Transformer la liste avec sentinelle en SubList
    SubList sublist;
    sublist.head = l->sentinel->next;
    sublist.tail = l->sentinel->previous;

    // Tri de la sous-liste en utilisant le tri fusion
    SubList sorted_sublist = list_mergesort(sublist, f);

    // Restaurer la structure de liste avec sentinelle
    l->sentinel->next = sorted_sublist.head;
    l->sentinel->previous = sorted_sublist.tail;

    sorted_sublist.head->previous = l->sentinel;
    sorted_sublist.tail->next = l->sentinel;

    return l;
}



SubList list_split(SubList l) {
    if (!l.head || l.head == l.tail) {
        // Si la sous-liste contient 0 ou 1 élément, pas besoin de la diviser
        return l;
    }

    LinkedElement* slow = l.head;
    LinkedElement* fast = l.head;

    // Utilisation de deux pointeurs pour trouver le milieu de la liste
    while (fast != l.tail && fast->next != l.tail) {
        slow = slow->next;
        fast = fast->next->next;
    }

    SubList right;
    right.head = slow->next;
    right.tail = l.tail;
    
    slow->next = NULL;  // Séparation des deux listes
    l.tail = slow;

    return right;
}



SubList list_merge(SubList left, SubList right, OrderFunctor f) {
    SubList merged;
    merged.head = NULL;
    merged.tail = NULL;

    LinkedElement* left_curr = left.head;
    LinkedElement* right_curr = right.head;
    LinkedElement* new_elem = NULL;

    while (left_curr && right_curr) {
        if (f(left_curr->value, right_curr->value)) {
            new_elem = left_curr;
            left_curr = left_curr->next;
        } else {
            new_elem = right_curr;
            right_curr = right_curr->next;
        }

        if (!merged.head) {
            merged.head = new_elem;
        } else {
            merged.tail->next = new_elem;
            new_elem->previous = merged.tail;
        }
        merged.tail = new_elem;
    }

    // Si l'une des deux listes a encore des éléments
    LinkedElement* remaining = (left_curr) ? left_curr : right_curr;
    while (remaining) {
        if (!merged.head) {
            merged.head = remaining;
        } else {
            merged.tail->next = remaining;
            remaining->previous = merged.tail;
        }
        merged.tail = remaining;
        remaining = remaining->next;
    }

    return merged;
}

SubList list_mergesort(SubList l, OrderFunctor f) {
    if (!l.head || l.head == l.tail) {
        return l; // Si la sous-liste contient 0 ou 1 élément, elle est déjà triée
    }

    // Diviser la liste en deux sous-listes
    SubList right = list_split(l);

    // Appel récursif pour trier les sous-listes gauche et droite
    SubList left_sorted = list_mergesort(l, f);
    SubList right_sorted = list_mergesort(right, f);

    // Fusionner les sous-listes triées
    return list_merge(left_sorted, right_sorted, f);
}


typedef struct {
    LinkedElement* head;
    LinkedElement* tail;
} SubList;


