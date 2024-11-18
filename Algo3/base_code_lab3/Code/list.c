/*-----------------------------------------------------------------*/
/*
 Licence Informatique - Structures de données
 Mathias Paulin (Mathias.Paulin@irit.fr)
 
 Implantation du TAD List vu en cours.
 */
/*-----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"

typedef struct s_LinkedElement {
	int value;
	struct s_LinkedElement* previous;
	struct s_LinkedElement* next;
} LinkedElement;

typedef struct {
    LinkedElement* head;
    LinkedElement* tail;
} SubList;

SubList list_split(SubList l);
SubList list_splot(SubList l);
SubList list_mergesort(SubList l, OrderFunctor f);
SubList list_merge(SubList left, SubList right, OrderFunctor f);
List* list_sort(List* l, OrderFunctor f); 
SubList list_mergesort(SubList l, OrderFunctor f);

struct s_List {
	LinkedElement* sentinel;
	int size;
};

typedef int (*Functor)(int value, void* env);
/*-----------------------------------------------------------------*/

List* list_create(void) {
    List* l = (List*) malloc(sizeof(List));
    LinkedElement* sentinel = (LinkedElement*) malloc(sizeof(LinkedElement));
    if (l!=NULL && sentinel!=NULL){
        sentinel->next = sentinel;
        sentinel->previous = sentinel;
        l->sentinel = sentinel;
        l->size = 0;
    } else {
        free(l);
        free(sentinel);
    }
    
    return l;
}


/*-----------------------------------------------------------------*/

List* list_push_back(List* l, int v) {
    LinkedElement* new_elem = (LinkedElement*) malloc(sizeof(LinkedElement));
    if (new_elem!=NULL){
        new_elem->value = v;
        new_elem->next = l->sentinel;
        new_elem->previous = l->sentinel->previous;
        l->sentinel->previous->next = new_elem;
        l->sentinel->previous = new_elem;
        l->size++;
    } else {
        free(new_elem);
    }
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
    if (new_elem!=NULL){
        new_elem->value = v;
        new_elem->next = l->sentinel->next;
        new_elem->previous = l->sentinel;
        l->sentinel->next->previous = new_elem;
        l->sentinel->next = new_elem;
        l->size++;
    } else {
        free(new_elem);
    }
    return l;
}


/*-----------------------------------------------------------------*/

int list_front(const List* l) {
    assert (!list_is_empty(l));
    return l->sentinel->next->value;
}


/*-----------------------------------------------------------------*/

int list_back(const List* l) {
    assert (!list_is_empty(l));
    return l->sentinel->previous->value;
}


/*-----------------------------------------------------------------*/

List* list_pop_front(List* l) {
    assert (!list_is_empty(l));
    LinkedElement* first = l->sentinel->next;
    l->sentinel->next = first->next;
    first->next->previous = l->sentinel;
    free(first);
    l->size--;
    return l;
}


/*-----------------------------------------------------------------*/

List* list_pop_back(List* l) {
    assert (!list_is_empty(l));
    LinkedElement* last = l->sentinel->previous;
    last->previous->next = l->sentinel;
    l->sentinel->previous = last->previous;
    free(last);
    l->size--;
    return l;
}


/*-----------------------------------------------------------------*/

List* list_insert_at(List* l, int p, int v) {
    assert (p <= l->size && p >= 0);
    LinkedElement* new_elem = (LinkedElement*) malloc(sizeof(LinkedElement));
    new_elem->value = v;
    
    LinkedElement* current = l->sentinel->next;
    for (;p;--p,current=current->next);

    new_elem->next = current;
    new_elem->previous = current->previous;
    current->previous->next = new_elem;
    current->previous = new_elem;
    l->size++;
    return l;
}


/*-----------------------------------------------------------------*/

List* list_remove_at(List* l, int p) {
    assert (!list_is_empty(l) && p < l->size && p >= 0);
    LinkedElement* current = l->sentinel->next;
    for (;p;--p,current=current->next);

    current->previous->next = current->next;
    current->next->previous = current->previous;
    free(current);
    l->size--;
    return l;
}


/*-----------------------------------------------------------------*/

int list_at(const List* l, int p) {
    assert (!list_is_empty(l) && p < l->size && p >= 0);
    LinkedElement* current = l->sentinel->next;
    for (;p;--p,current=current->next);
    return current->value;
}


/*-----------------------------------------------------------------*/

bool list_is_empty(const List* l) {
    return (l== NULL || l->size == 0);
}


/*-----------------------------------------------------------------*/

int list_size(const List* l) {
    if (list_is_empty(l)){
        return 0;
    }
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

#if 0
// ############################################################ 1er Essai ######################################################################## 
SubList list_split(SubList l) {
    SubList right = {NULL, NULL};  // Creation de la demi liste droite

    // Si la liste est vide on retourne une liste vide
    if (!l.head || !l.tail || l.head == l.tail) {
        return right;
    }
    
    // Cas ou la liste ne contient seulement 2 éléments
    if (l.head->next == l.tail) {
        right.head = l.tail;
        right.tail = l.tail;
        l.tail = l.head;
        l.head->next = NULL;
        right.head->previous = NULL;
        return right;
    }

    // Trouver le centre avec un curseur rapide et un curseur lent
    LinkedElement* slow = l.head;
    LinkedElement* fast = l.head->next;
    
    while (fast->next != l.tail && fast->next->next != l.tail) { // j'ai également essayer (fast!=l.tail->previous &&fast!=l.tail->previous->previous)
        slow = slow->next;
        fast = fast->next->next;
    }

    // Le curseur lent pointe le milieu
    // Mettre a jour la liste de droite
    right.head = slow->next;
    right.tail = l.tail;
    
    // Mettre a jour la liste de gauche
    l.tail = slow;
    l.tail->next = NULL;
    
    if (right.head) {
        right.head->previous = NULL;
    }

    return right;
}
#endif
#if 0
//#################################################### 2ème Essai #############################################################
SubList list_split(SubList l) { 
    if (!l.head || !l.tail || l.head == l.tail) {
        SubList empty = { NULL, NULL };
        return empty;
    }

    int count = 0;
    LinkedElement* current = l.head;
    while (current) {
        count++;
        current = current->next;
    }

    int middle = count / 2;

    // Parcourir jusqu'à la moitié
    current = l.head;
    for (int i = 0; i < middle - 1; i++) {
        current = current->next;
    }

    // Définir la sous-liste droite
    SubList right;
    right.head = current->next;
    right.tail = l.tail;

    // Séparer la sous-liste gauche de la droite
    current->next = NULL;
    right.head->previous = NULL;
    l.tail = current;

    // Retourner la sous-liste droite
    return right;
}
#endif

#if 0
//################################################### 3ème Essai #############################################################################
SubList list_split(SubList l) {
    if (!l.head || l.head == l.tail) {
        return l;
    }

    LinkedElement* start = l.head;
    LinkedElement* end = l.tail;

    while (start->next != end && start->next !=end->previous) {
        start = start->next;
        end = end->previous;
    }

    SubList right;
    if(start-> next == end->previous){
        right.head=end->previous;
    }else{right.head = end;}
    
    right.tail = l.tail;
    l.tail = start;
    start->next = NULL;
    right.head->previous = NULL;
    

    return right;
}
#endif


SubList list_merge(SubList left, SubList right, OrderFunctor f) {
    if (left.head == NULL){
        return right;
    }
    if (right.head==NULL){
        return left;
    }

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

    LinkedElement* remaining;
    if (left_curr){
        remaining = left_curr;
    } else {remaining = right_curr;}

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


List* list_sort(List* l, OrderFunctor f) {
    if (list_is_empty(l)) {
        return l;
    }

    SubList sublist;
    sublist.head = l->sentinel->next;
    sublist.tail = l->sentinel->previous;
    
    SubList sorted_sublist = list_mergesort(sublist, f);

    l->sentinel->next = sorted_sublist.head;
    l->sentinel->previous = sorted_sublist.tail;

    sorted_sublist.head->previous = l->sentinel;
    sorted_sublist.tail->next = l->sentinel;

    return l;
}

SubList list_mergesort(SubList l, OrderFunctor f) {
    if (!l.head || !l.head->next) {
        return l;
    }
    SubList right = list_split(l);
    return list_merge(list_mergesort(l, f), list_mergesort(right, f), f);
}


