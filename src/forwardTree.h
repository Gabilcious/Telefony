#ifndef __forwardTree_h__
#define __forwardTree_h__

#ifndef __empty__
#define __empty__
static const char EMPTY[] = "empty";
static const int SIZE = 10;
#endif

typedef struct ForwardTree {
    char *val;
    char *id;
    int x;
    struct ForwardTree **next;
    struct ForwardTree *prev;
} ForwardTree;

ForwardTree * forNewNode(ForwardTree *prev, int x, const char *val);

void forRemoveNode(ForwardTree *head);

void forClearFromNode(ForwardTree *head);

void forRemoveAllNodes(ForwardTree *head);

#endif
