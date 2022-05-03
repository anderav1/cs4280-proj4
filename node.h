// Lexi Anderson
// CS4280, Proj 4
// node.h

#ifndef NODE_H
#define NODE_H

#include "token.h"

typedef struct node_t {
  std::string label;
  token_t decor;
  bool isLeaf = false;
  // children nodes
  node_t* ch1 = nullptr;
  node_t* ch2 = nullptr;
  node_t* ch3 = nullptr;
  node_t* ch4 = nullptr;
  node_t* ch5 = nullptr;
  node_t* ch6 = nullptr;
  node_t* ch7 = nullptr;
} node_t;


#endif
