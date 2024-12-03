#include <stdlib.h>
#include <assert.h>
#include "skiplist.h"
#include "rng.h"

typedef struct SkipListNode {
    int value;
    struct SkipListNode **next;
    struct SkipListNode **prev;
} SkipListNode;

struct s_SkipList {
    SkipListNode *head;
    int nb_levels;
    unsigned int size;
    RNG rng;
};

SkipList* skiplist_create(int nb_levels) {
    SkipList *sl = (SkipList *)malloc(sizeof(SkipList));
    sl->nb_levels = nb_levels;
    sl->size = 0;
    sl->rng = rng_initialize(0, nb_levels);

    // la sentinelle
    sl->head = (SkipListNode *)malloc(sizeof(SkipListNode));
    sl->head->value = -1;
    sl->head->next = (SkipListNode **)malloc(sizeof(SkipListNode *) * nb_levels);
    sl->head->prev = (SkipListNode **)malloc(sizeof(SkipListNode *) * nb_levels);

    for (int i = 0; i < nb_levels; i++) {
        sl->head->next[i] = NULL;
        sl->head->prev[i] = NULL;
    }

    return sl;
}

void skiplist_delete(SkipList** d) {
    if (*d == NULL) return;

    SkipList* list = *d;
    SkipListNode* current = list->head->next[0];

    while (current != NULL) {
        SkipListNode* temp = current;
        current = current->next[0];

        free(temp->next);
        free(temp->prev);
        free(temp);
    }

    free(list->head->next);
    free(list->head->prev);
    free(list->head); 

    free(list);
    *d = NULL;
}




unsigned int skiplist_size(const SkipList *d) {
    return d ? d->size : 0;
}


int skiplist_at(const SkipList *d, unsigned int i) {
    if (!d || i >= d->size) return -1;

    SkipListNode *current = d->head->next[0];
    for (unsigned int index = 0; current && index < i; index++) {
        current = current->next[0];
    }

    return current ? current->value : -1;
}


void skiplist_map(const SkipList *d, ScanOperator f, void *user_data) {
    if (!d || !f) return;

    SkipListNode *current = d->head->next[0];
    while (current) {
        f(current->value, user_data);
        current = current->next[0];
    }
}


SkipList* skiplist_insert(SkipList *d, int value) {
    if (!d) return NULL;
    // Eviter d'avoir plusieurs node d'une valeur
    if (skiplist_search(d, value, NULL)) {
        return d;
    }
    // Nombre de niveaux aléatoire
    int level = rng_get_value(&(d->rng));

    SkipListNode *new_node = (SkipListNode *)malloc(sizeof(SkipListNode));
    new_node->value = value;
    new_node->next = (SkipListNode **)malloc(sizeof(SkipListNode *) * (level + 1));
    new_node->prev = (SkipListNode **)malloc(sizeof(SkipListNode *) * (level + 1));
    for (int i = 0; i <= level; i++) {
        new_node->next[i] = NULL;
        new_node->prev[i] = NULL;
    }

    // Insérer à chaque niveau
    SkipListNode *current = d->head;
    for (int i = level; i >= 0; i--) {
        while (current->next[i] && current->next[i]->value < value) {
            current = current->next[i];
        }
        //Mise a jour des pointeurs
        new_node->next[i] = current->next[i];
        if (current->next[i]) {
            current->next[i]->prev[i] = new_node;
        }

        current->next[i] = new_node;
        new_node->prev[i] = current;
    }

    d->size++;
    return d;
}

bool skiplist_search(const SkipList* d, int value, unsigned int *nb_operations) {
    SkipListNode* current = d->head;
    int level = d->nb_levels-1;
    unsigned int operations = 0;

    while (level >= 0) {
        // traverse horizontale
        while (current->next[level] != NULL && current->next[level]->value < value) {
            current = current->next[level];
            operations++;
        }

        // Verifie le prochain node à chaque niveau
        if (current->next[level] != NULL && current->next[level]->value == value) {
            if (nb_operations != NULL) {
                *nb_operations = operations+1;// +1 car la valeur recherchée est dans le prochain
            }
            return true; 
        }
        level--;
    }

    if (nb_operations != NULL) {
        *nb_operations = operations+1;
    }
    return false;
}

bool skiplist_remove(SkipList* d, int value) {
    if (!d) return false;

    SkipListNode* update[d->nb_levels];//dernier node de chaque niveau avant la valeur
    SkipListNode* current = d->head;

    for (int level = d->nb_levels - 1; level >= 0; level--) {
        while (current->next[level] && current->next[level]->value < value) {
            current = current->next[level];
        }
        update[level] = current;
    }

    current = current->next[0];
    if (!current || current->value != value) {
        return false;
    }

    //Mise a jour du dernier node de chaque niveau ou existe la valeur a supprimer
    for (int level = 0; level < d->nb_levels; level++) {
        if (update[level]->next[level] == current) {
            update[level]->next[level] = current->next[level];
            if (current->next[level]) {
                current->next[level]->prev[level] = update[level];
            }
        } else {
            break;
        }
    }
    free(current->next);
    free(current->prev);
    free(current);
    d->size--;

    return true;
}




//############################################################################  Iterator   ################################################################################

struct s_SkipListIterator {
    SkipList* list;
    SkipListNode* current;
    IteratorDirection direction;
};

SkipListIterator* skiplist_iterator_create(SkipList* d, IteratorDirection w) {
    SkipListIterator* it = (SkipListIterator*)malloc(sizeof(SkipListIterator));
    it->list = d;
    it->direction = w;

    if (w == FORWARD_ITERATOR) {
        it->current = d->head->next[0];
    } else { //BACKWARD_ITERATOR
        it->current = d->head;
        while (it->current->next[0] != NULL) {
            it->current = it->current->next[0];
        }
    }
    return it;
}

void skiplist_iterator_delete(SkipListIterator** it) {
    if (it != NULL && *it != NULL) {
        free(*it);
        *it = NULL;
    }
}

SkipListIterator* skiplist_iterator_begin(SkipListIterator* it) {
    if (it->direction == FORWARD_ITERATOR) {
        it->current = it->list->head->next[0];
    } else { // BACKWARD_ITERATOR
        it->current = it->list->head;
        while (it->current->next[0] != NULL) {
            it->current = it->current->next[0];
        }
    }
    return it;
}

bool skiplist_iterator_end(SkipListIterator* it) {
    return it->current == NULL;
}

SkipListIterator* skiplist_iterator_next(SkipListIterator* it) {
    if (it->direction == FORWARD_ITERATOR) {
        if (it->current != NULL) {
            it->current = it->current->next[0];
        }
    } else {  // BACKWARD_ITERATOR
        if (it->current != NULL) {
            it->current = it->current->prev[0];
        }
    }
    return it;
}

int skiplist_iterator_value(SkipListIterator* it) {
    if (it->current != NULL) {
        return it->current->value;
    }
    return -1;
}
