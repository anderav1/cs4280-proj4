// Lexi Anderson
// CS4280, Proj 4
// list.cpp

#include "list.h"
#include "token.h"

// Create new empty list
list_t* createList() {
  list_t* lst = new list_t();
  
  lst->head = nullptr;
  lst->tail = nullptr;
  
  return lst;
}

// Create new list node
listnode_t* newListNode(token_t tkn) {
  listnode_t* node = new listnode_t();
  
  node->tkn = tkn;
  node->next = nullptr;
  
  return node;
}

// Add node to list
void addToList(list_t* lst, token_t tkn) {
  listnode_t* node = newListNode(tkn);
  
  if (lst->head == nullptr) {
    lst->head = node;
    lst->tail = node;
    return;
  }
  
  lst->tail->next = node;
  lst->tail = node;
}

// Check if token instance is in list
bool isInList(list_t* lst, token_t tkn) {
  listnode_t* ptr = lst->head;
  
  while (ptr != nullptr) {
    if (ptr->tkn.str == tkn.str) return true;
    ptr = ptr->next;
  }
  
  return false;
}

// Delete the linked list
void deleteList(list_t* lst) {
  listnode_t *curr, *next;
  curr = lst->head;
  
  while (curr != nullptr) {
    next = curr->next;
    delete curr;
    curr = next;
  }
}
