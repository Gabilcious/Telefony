#ifndef __reverseTree_h__
#define __reverseTree_h__
#include "list.h"

#ifndef __empty__
#define __empty__
static const char EMPTY[] = "empty";
static const int SIZE = 10;
#endif

typedef struct ReverseTree {
    char *val;
    List *id;
    int x;
    int size;
    struct ReverseTree **next;
    struct ReverseTree *prev;
} ReverseTree;

ReverseTree * revNewNode(ReverseTree *prev, int x, const char *val);

inline void revDeleteFromNode(ReverseTree *head, char *id) {
    removeConcrete(head->id, id);
}

void revRemoveNode(ReverseTree *head);

void revRemoveConcreteNode(ReverseTree *head, char *id, char *id2);

void revClearFromNode(ReverseTree *head);

void revRemoveAllNodes(ReverseTree *head);

#endif
