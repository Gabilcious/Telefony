#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "forwardTree.h"

ForwardTree * forNewNode(ForwardTree *prev, int x, const char *val) {
    ForwardTree *element;
    element = (ForwardTree *)malloc(sizeof(ForwardTree));
    element->x = x;
    //czy trzeba ifa?
    if(strcmp(val,EMPTY)) {
        element->val = malloc(strlen(val)+1);
    } else element->val =  malloc(6);
    strcpy(element->val, val);
    element->next = (ForwardTree **)malloc(SIZE*sizeof(ForwardTree *));
    element->id = (char *)malloc(6*sizeof(char));
    strcpy(element->id, EMPTY);
    element->prev = prev;
    int i;
    for (i = 0; i < SIZE; i++) element->next[i] = NULL;
    return element;
}

void forRemoveNode(ForwardTree *head) {
    if(head == NULL) return;
    free(head->next);
    free(head->id);
    free(head->val);
    if(head->prev != NULL) (head->prev)->next[head->x] = NULL;
    free(head);
}

void forClearFromNode(ForwardTree *head) {
    if(head == NULL || head->x == -1) return;
    int i;
    for(i = 0; i < SIZE; i++) if(head->next[i] != NULL) return;
    if(strcmp(head->id, EMPTY)) return;
    ForwardTree *prev = head->prev;
    forRemoveNode(head);
    forClearFromNode(prev);
}

void forRemoveAllNodes(ForwardTree *head) {
    int i;
    if(head == NULL) return;
    for(i = 0; i < SIZE; i++)
        if(head->next[i] != NULL) forRemoveAllNodes(head->next[i]);
    forRemoveNode(head);
}
    
