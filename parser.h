// Lexi Anderson
// CS4280, Proj 3
// parser.h

#ifndef PARSER_H
#define PARSER_H

#include "node.h"

node_t* parser(std::istream &input);
void error();
node_t* getNode(std::string);
node_t* getLeafNode(std::string);
node_t* getDecoratedNode(std::string, std::string);
void deleteTree(node_t*);

// nonterminals
node_t* S();
node_t* R();
node_t* E();
node_t* A();
node_t* B();
node_t* C();
node_t* D();
node_t* F();
node_t* G();
node_t* T();
node_t* V();
node_t* H();
node_t* J();
node_t* K();
node_t* L();
node_t* W();
node_t* Z();


#endif
