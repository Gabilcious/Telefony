#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tree.h"

Tree * newNode(Tree *prev, int x) {
    Tree *element;
    if((element = (Tree *)malloc(sizeof(Tree))) == NULL) return NULL;
    element->x = x;
    if((element->next = (Tree **)malloc(SIZE*sizeof(Tree *))) == NULL) {
        free(element);
        return NULL;
    }
        char *c;
        c = (char *)malloc(6*sizeof(char));
        strcpy(c,EMPTY);
        element->id = c;
    element->prev = prev;
    int i;
    for (i = 0; i < SIZE; i++) element->next[i] = NULL;
    return element;
}

void removeNode(Tree *head) {
    if(head == NULL) return;
    int i;
    free(head->next);
    free(head->id);
    if(head->prev != NULL) (head->prev)->next[head->x] = NULL;
    free(head);
}

void removeConcreteNode(Tree *head, char *id) {
    if(!strcmp(id,EMPTY)) return;
    Tree *it = head;
    int i = 0;
    while(id[i] != 0) {
        int x = (int)id[i] - 48;
        it = it->next[x];
        i++;
    }
    free(it->id);
    char *c;
    c = (char *)malloc(6*sizeof(char));
    strcpy(c,EMPTY);
    it->id = c;
    clearFromNode(it);
}

void clearFromNode(Tree *head) {
    if(head == NULL || head->x == -1) return;
    int i;
    for(i = 0; i < SIZE; i++) if(head->next[i] != NULL) return;
    if(strcmp(head->id,EMPTY)) return;
    Tree *prev = head->prev;
    removeNode(head);
    clearFromNode(prev);
}

void removeAllNodes(Tree *head) {
    int i;
    if(head == NULL) return;
    for(i = 0; i < SIZE; i++)
        if(head->next[i] != NULL) removeAllNodes(head->next[i]);
    removeNode(head);
}
    
