#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include <string.h>

//dodać ify mallocujące
List *newList(const char *id, List *next) {
    List *element;
    element = malloc(sizeof(List));
    element->id = malloc(strlen(id)+1);
    strcpy(element->id,id);
    element->next = next;
    return element;
}


void add(List *head, const char *id) {
    List *element;
    element = newList(id, head->next);
    head->next = element;
}


void removeNext(List *head) {
    List *element = head->next;
    head->next = element->next;
    free(element->id);
    free(element);
}

void removeConcrete(List *head, char *id) {
    List *current = head;
    while (current->next != NULL) {
        if (!strcmp((current->next)->id, id)) {
            removeNext(current);
        }
        else {
            current = current->next;
        }
    }
}

void clear(List *head) {
    while (head->next != NULL) {
        removeNext(head);
    }
    free(head->id);
    free(head);
    head = NULL;
}

bool exist(List *head, char *id) {
    List *current = head;

    while (current->next != NULL && strcmp(current->id, id)) {
        current = current->next;
    }

    return !strcmp(current->id, id);
}
