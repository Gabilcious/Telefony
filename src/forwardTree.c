#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "forwardTree.h"

ForwardTree *forNewNode(ForwardTree *prev, int lastDigFrom, const char *from) {
    ForwardTree *element;
    element = (ForwardTree *)malloc(sizeof(ForwardTree));
	if (element == NULL) {
		return NULL;
	}
    element->lastDigFrom = lastDigFrom;
    element->prev = prev;
	element->from = (char *)malloc(strlen(from) + 1);
    strcpy(element->from, from);
	element->next = (ForwardTree **)malloc(SIZE * sizeof(ForwardTree *));
	element->to = (char *)malloc(6);
    strcpy(element->to, EMPTY);
	int i;
    for (i = 0; i < SIZE; i++) {
		element->next[i] = NULL;
	}
    return element;
}

void forRemoveNode(ForwardTree *head) {
    if (head == NULL) {
		return;
	}
    free(head->next);
    free(head->to);
    free(head->from);
    if (head->prev != NULL) {
		(head->prev)->next[head->lastDigFrom] = NULL;
	}
    free(head);
}

void forClearFromNode(ForwardTree *head) {
    if (head == NULL || head->lastDigFrom == -1) {
		return;
	}
    int i;
    for (i = 0; i < SIZE; i++) {
		if (head->next[i] != NULL) {
			return;
		}
	}
    if (strcmp(head->to, EMPTY)) {
		return;
	}
    ForwardTree *prev = head->prev;
    forRemoveNode(head);
    forClearFromNode(prev);
}

void forRemoveAllNodes(ForwardTree *head) {
    if (head == NULL) {
		return;
	}
	int i;
    for (i = 0; i < SIZE; i++) {
        if (head->next[i] != NULL) {
			forRemoveAllNodes(head->next[i]);
		}
	}
    forRemoveNode(head);
} 
