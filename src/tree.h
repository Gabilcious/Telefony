#ifndef __tree_h__
#define __tree_h__

static const char EMPTY[] = "empty";
static const int SIZE = 10;

typedef struct Tree {
    char *id;
    int x;
    struct Tree **next;
    struct Tree *prev;
} Tree;

Tree * newNode(Tree *prev, int x);

void removeNode(Tree *head);

void removeConcreteNode(Tree *head, char *id);

void clearFromNode(Tree *head);

void removeAllNodes(Tree *head);

#endif
