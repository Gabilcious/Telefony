#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "reverse_tree.h"

ReverseTree *revNewNode(ReverseTree *prev, int lastDigTo, const char *to) {
	ReverseTree *element;
    element = (ReverseTree *)malloc(sizeof(ReverseTree));
    if (element == NULL) {
		return NULL;
	}
	element->lastDigTo = lastDigTo;
    element->forwardCount = 0;
    element->prev = prev;
    element->to = (char *)malloc(strlen(to) + 1);
    strcpy(element->to, to);
    element->next = (ReverseTree **)malloc(SIZE * sizeof(ReverseTree *));
    element->from = newList(EMPTY, NULL);
    int i;
    for (i = 0; i < SIZE; i++) {
		element->next[i] = NULL;
	}
	return element;
}

void revDeleteFromNode(ReverseTree *head, char *from) {
    removeConcrete(head->from, from);
}

void revRemoveNode(ReverseTree *head) {
    if (head == NULL) {
		return;
	}
    free(head->next);
    clear(head->from);
    free(head->to);
    if (head->prev != NULL) {
		(head->prev)->next[head->lastDigTo] = NULL;
	}
    free(head);
}

void revClearFromNode(ReverseTree *head) {
    if (head == NULL || head->lastDigTo == -1) {
		return;
	}
    int i;
    for (i = 0; i < SIZE; i++) {
		if (head->next[i] != NULL) {
			return;
		}
	}
    if (!isEmpty(head->from)) {
		return;
	}
    ReverseTree *prev = head->prev;
    revRemoveNode(head);
    revClearFromNode(prev);
}

void revRemoveConcreteNode(ReverseTree *head, char *to, char *from) {
    if (!strcmp(from, EMPTY)) {
		return;
	}
    ReverseTree *it = head;
    int i = 0;
    while (to[i] != 0) {
        int digit = (int)to[i] - 48;
        it = it->next[digit];
        i++;
    }
    revDeleteFromNode(it, from);
    head->forwardCount--;
    revClearFromNode(it);
}

void revRemoveAllNodes(ReverseTree *head) {
    int i;
    if (head == NULL) {
		return;
	}
    for (i = 0; i < SIZE; i++) {
        if (head->next[i] != NULL) {
			revRemoveAllNodes(head->next[i]);
		}
	}
    revRemoveNode(head);
}
    
