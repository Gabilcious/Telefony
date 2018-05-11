#ifndef __list_h__
#define __list_h__
#include <stdbool.h>

typedef struct List {
    char *id;
    struct List *next;
} List;

List * newList();

void add(List *head, const char *id);

void removeNext(List *head);

void removeConcrete(List *head, char *id);

void clear(List *head);

inline bool isEmpty(List *head) {
    return (head->next == NULL);
}

bool exist(List *head, char *id);

#endif
