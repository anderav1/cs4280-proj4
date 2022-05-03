// Lexi Anderson
// CS4280, Proj 3
// list.h

#ifndef LIST_H
#define LIST_H

#include "token.h"

typedef struct listnode_t {
  token_t tkn;
  struct listnode_t* next;
} listnode_t;

typedef struct {
  listnode_t *head, *tail;
} list_t;


list_t* createList();
listnode_t* newListNode(token_t);
void addToList(list_t*, token_t);
bool isInList(list_t*, token_t);
void deleteList(list_t*);

#endif
