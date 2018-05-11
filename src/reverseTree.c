#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "reverseTree.h"

ReverseTree * revNewNode(ReverseTree *prev, int x, const char *val) {
    ReverseTree *element;
    element = (ReverseTree *)malloc(sizeof(ReverseTree));
    element->x = x;
    element->size = 0;
    if(strcmp(val,EMPTY)) {
        element->val = malloc(strlen(val)+1);
    } else element->val =  malloc(6);
    strcpy(element->val, val);
    element->next = (ReverseTree **)malloc(SIZE*sizeof(ReverseTree *));
    element->id = newList(EMPTY, NULL);
    element->prev = prev;
    int i;
    for (i = 0; i < SIZE; i++) element->next[i] = NULL;
    return element;
}

void revRemoveNode(ReverseTree *head) {
    if(head == NULL) return;
    free(head->next);
    clear(head->id);
    free(head->val);
    if(head->prev != NULL) (head->prev)->next[head->x] = NULL;
    free(head);
}

void revRemoveConcreteNode(ReverseTree *head, char *val, char *id) {
    if(!strcmp(id,EMPTY)) return;
    ReverseTree *it = head;
    int i = 0;
    while(val[i] != 0) {
        int x = (int)val[i] - 48;
        it = it->next[x];
        i++;
    }
    revDeleteFromNode(it, id);
    head->size--;
    revClearFromNode(it);
}

void revClearFromNode(ReverseTree *head) {
    if(head == NULL || head->x == -1) return;
    int i;
    for(i = 0; i < SIZE; i++) if(head->next[i] != NULL) return;
    if(!isEmpty(head->id)) return;
    ReverseTree *prev = head->prev;
    revRemoveNode(head);
    revClearFromNode(prev);
}

void revRemoveAllNodes(ReverseTree *head) {
    int i;
    if(head == NULL) return;
    for(i = 0; i < SIZE; i++)
        if(head->next[i] != NULL) revRemoveAllNodes(head->next[i]);
    revRemoveNode(head);
}
    
