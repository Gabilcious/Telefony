#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include <string.h>

List *newList(const char *id, List *next) {
    List *element;
    element = (List *)malloc(sizeof(List));
    if (element == NULL) {
		return NULL;
	}
	element->id = (char *)malloc(strlen(id) + 1);
    strcpy(element->id, id);
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

bool isEmpty(List *head) {
    return (head->next == NULL);
}
