// Lexi Anderson
// CS4280, Proj 3
// parser.cpp

#include <iostream>

#include "list.h"
#include "node.h"
#include "parser.h"
#include "scanner.h"
#include "semantics.h"
#include "token.h"

#include <iostream>

token_t tkn;
std::istream* fp;
list_t* symbolTable;

node_t* parser(std::istream &input) {
  node_t* root;
  symbolTable = createList();

  fp = &input;
  tkn = scanner(*fp);
  root = S();
  
  if (tkn.id != EOF_TK) error();
  else std::cout << "Successfully parsed program" << std::endl;
  
  deleteList(symbolTable);
  
  return root;
}

// Abort program on parsing error
void error() {
  std::cout << "Parsing error at " << tkn.id << " token \'" << tkn.str << "\' on line " << tkn.line << std::endl;
  exit(0);
}

// Create an empty node with label
node_t* getNode(std::string label) {
  node_t* node = new node_t();
  node->label = label;
  // all ptr members should be null
  
  return node;
}

node_t* getLeafNode(std::string label) {
  node_t* node = new node_t();
  node->label = label;
  node->isLeaf = true;
  
  return node;
}

node_t* getDecoratedNode(std::string label, token_t decor) {
  node_t* node = new node_t();
  node->label = label;
  node->isLeaf = true;
  node->decor = decor;
  
  return node;
}

// Release memory allocated to parse tree
void deleteTree(node_t* root) {
  if (root == nullptr) return;
  
  deleteTree(root->ch1);
  deleteTree(root->ch2);
  deleteTree(root->ch3);
  deleteTree(root->ch4);
  deleteTree(root->ch5);
  deleteTree(root->ch6);
  deleteTree(root->ch7);
  
  delete root;
  root = nullptr;
}

// nonterminal functions

// <S> -> Name Id Spot Id <R> <E>
node_t* S() {
  node_t* node = getNode("<S>");
  if (tkn.id == KEYWD_TK && tkn.str == "Name") {
    node->ch1 = getLeafNode(tkn.str);
    tkn = scanner(*fp);
    if (tkn.id == ID_TK) {
      // TODO: check id semantics
      insert(symbolTable, tkn); //
      node->ch2 = getDecoratedNode("Identifier", tkn);
      tkn = scanner(*fp);
      if (tkn.id == KEYWD_TK && tkn.str == "Spot") {
        node->ch3 = getLeafNode(tkn.str);
        tkn = scanner(*fp);
        if (tkn.id == ID_TK) {
          insert(symbolTable, tkn); //
          node->ch4 = getDecoratedNode("Identifier", tkn);
          tkn = scanner(*fp);
          node->ch5 = R();
          node->ch6 = E();
        } else error();
      } else error();
    } else error();
  } else error();
  
  return node;
}

// <R> -> Place <A> <B> Home
node_t* R() {
  node_t* node = getNode("<R>");

  if (tkn.id == KEYWD_TK && tkn.str == "Place") {
    node->ch1 = getLeafNode(tkn.str);
    tkn = scanner(*fp);
    node->ch2 = A();
    node->ch3 = B();
    if (tkn.id == KEYWD_TK && tkn.str == "Home") {
      node->ch4 = getLeafNode(tkn.str);
      tkn = scanner(*fp);
    } else error();
  } else error();
  
  return node;
}

// <E> -> Show Id
node_t* E() { // working
  node_t* node = getNode("<E>");

  if (tkn.id == KEYWD_TK && tkn.str == "Show") {
    node->ch1 = getLeafNode(tkn.str);
    tkn = scanner(*fp);
    if (tkn.id == ID_TK) {
      if (!verify(symbolTable, tkn)) error(tkn, ID_UNDEFINED); //
      node->ch2 = getDecoratedNode("Identifier", tkn);
      tkn = scanner(*fp);
    } else error();
  } else error();
  
  return node;
}

// <A> -> Name Id
node_t* A() {
  node_t* node = getNode("<A>");

  if (tkn.id == KEYWD_TK && tkn.str == "Name") {
    node->ch1 = getLeafNode(tkn.str);
    tkn = scanner(*fp);
    if (tkn.id == ID_TK) {
      insert(symbolTable, tkn); //
      node->ch2 = getDecoratedNode("Identifier", tkn);
      tkn = scanner(*fp);
    } else error();
  } else error();
  
  return node;
}

// <B> -> empty | . <C> . <B> | <D> <B>
node_t* B() {
  node_t* node = getNode("<B>");

  if (tkn.id == OP_TK && tkn.str == ".") { // <B> -> . <C> . <B>
    node->ch1 = getLeafNode(tkn.str);
    tkn = scanner(*fp);
    node->ch2 = C();
    if (tkn.id == OP_TK && tkn.str == ".") {
      node->ch3 = getLeafNode(tkn.str);
      tkn = scanner(*fp);
      node->ch4 = B();
    } else error();
  } else if ((tkn.id == OP_TK && (tkn.str == "/" || tkn.str == "{")) || (tkn.id == KEYWD_TK && (tkn.str == "Assign" || tkn.str == "Spot" || tkn.str == "Move" || tkn.str == "Flip" || tkn.str == "Show"))) { // <B> -> <D> <B>
    node->ch1 = D();
    node->ch2 = B();
  } // else <B> nullable
  
  return node;
}

// <C> -> <F> | <G>
node_t* C() {
  node_t* node = getNode("<C>");

  if (tkn.id == OP_TK && tkn.str == "{") { // <C> -> <F>
    node->ch1 = F();
  } else if (tkn.id == KEYWD_TK && tkn.str == "Here") { // <C> -> <G>
    node->ch1 = G();
  } else error();
  
  return node;
}

// <D> -> <H> | <J> | <K> | <L> | <E> | <F>
node_t* D() {
  node_t* node = getNode("<D>");

  if (tkn.id == OP_TK) {
    if (tkn.str == "/") { // <D> -> <H>
      node->ch1 = H();
    } else if (tkn.str == "{") { // <D> -> <F>
      node->ch1 = F();
    } else error();
  } else if (tkn.id == KEYWD_TK) {
    if (tkn.str == "Assign") { // <D> -> <J>
      node->ch1 = J();
    } else if (tkn.str == "Spot" || tkn.str == "Move") { // <D> -> <K>
      node->ch1 = K();
    } else if (tkn.str == "Flip") { // <D> -> <L>
      node->ch1 = L();
    } else if (tkn.str == "Show") { // <D> -> <E>
      node->ch1 = E();
    } else error();
  } else error();
  
  return node;
}

// <F> -> { If Id <T> <W> <D> } | { Do Again <D> <T> <W> }
node_t* F() {
  node_t* node = getNode("<F>");

  if (tkn.id == OP_TK && tkn.str == "{") {
    node->ch1 = getLeafNode(tkn.str);
    tkn = scanner(*fp);
    if (tkn.id == KEYWD_TK) {
      if (tkn.str == "If") { // <F> -> { If Id <T> <W> <D> }
        node->ch2 = getLeafNode(tkn.str);
        tkn = scanner(*fp);
        if (tkn.id == ID_TK) {
          if (!verify(symbolTable, tkn)) error(tkn, ID_UNDEFINED); //
          node->ch3 = getDecoratedNode("Identifier", tkn);
          tkn = scanner(*fp);
          node->ch4 = T();
          node->ch5 = W();
          node->ch6 = D();
          // finish after if-statement
        } else error();
      } else if (tkn.str == "Do") { // <F> -> { Do Again <D> <T> <W> }
        node->ch2 = getLeafNode(tkn.str);
        tkn = scanner(*fp);
        if (tkn.id == KEYWD_TK && tkn.str == "Again") {
          node->ch3 = getLeafNode(tkn.str);
          tkn = scanner(*fp);
          node->ch4 = D();
          node->ch5 = T();
          node->ch6 = W();
          // finish after if-statement
        } else error();
      } else error();
    } else error();
  } else error();
  
  // closing brace for both productions
  if (tkn.id == OP_TK && tkn.str == "}") {
    node->ch7 = getLeafNode(tkn.str);
    tkn = scanner(*fp);
  } else error();
  
  return node;
}

// <G> -> Here Num There
node_t* G() {
  node_t* node = getNode("<G>");

  if (tkn.id == KEYWD_TK && tkn.str == "Here") {
    node->ch1 = getLeafNode(tkn.str);
    tkn = scanner(*fp);
    if (tkn.id == NUM_TK) {
      node->ch2 = getDecoratedNode("Number", tkn);
      tkn = scanner(*fp);
      if (tkn.id == KEYWD_TK && tkn.str == "There") {
        node->ch3 = getLeafNode(tkn.str);
        tkn = scanner(*fp);
      } else error();
    } else error();
  } else error();
  
  return node;
}

// <T> -> << | <-
node_t* T() {
  node_t* node = getNode("<T>");

  if (tkn.id == OP_TK) {
    if (tkn.str == "<<" || tkn.str == "<-") {
      node->ch1 = getLeafNode(tkn.str);
      tkn = scanner(*fp);
    } else error();
  } else error();
  
  return node;
}

// <V> -> + | % | &
node_t* V() {
  node_t* node = getNode("<V>");

  if (tkn.id == OP_TK) {
    if (tkn.str == "+" || tkn.str == "%" || tkn.str == "&") {
      node->ch1 = getLeafNode(tkn.str);
      tkn = scanner(*fp);
    } else error();
  } else error();
  
  return node;
}

// <H> -> / <Z>
node_t* H() {
  node_t* node = getNode("<H>");

  if (tkn.id == OP_TK && tkn.str == "/") {
    node->ch1 = getLeafNode(tkn.str);
    tkn = scanner(*fp);
    node->ch2 = Z();
  } else error();
  
  return node;
}

// <J> -> Assign Id <D>
node_t* J() {
  node_t* node = getNode("<J>");

  if (tkn.id == KEYWD_TK && tkn.str == "Assign") {
    node->ch1 = getLeafNode(tkn.str);
    tkn = scanner(*fp);
    if (tkn.id == ID_TK) {
      if (!verify(symbolTable, tkn)) error(tkn, ID_UNDEFINED); //
      node->ch2 = getDecoratedNode("Identifier", tkn);
      tkn = scanner(*fp);
      node->ch3 = D();
    } else error();
  } else error();
  
  return node;
}

// <K> -> Spot Num Show Num | Move Id Show Id
node_t* K() {
  node_t* node = getNode("<K>");

  if (tkn.id == KEYWD_TK) {
    if (tkn.str == "Spot") { // <K> -> Spot Num Show Num
      node->ch1 = getLeafNode(tkn.str);
      tkn = scanner(*fp);
      if (tkn.id == NUM_TK) {
        node->ch2 = getDecoratedNode("Number", tkn);
        tkn = scanner(*fp);
        if (tkn.id == KEYWD_TK && tkn.str == "Show") {
          node->ch3 = getLeafNode(tkn.str);
          tkn = scanner(*fp);
          if (tkn.id == NUM_TK) {
            node->ch4 = getDecoratedNode("Number", tkn);
            tkn = scanner(*fp);
          } else error();
        } else error();
      } else error();
    } else if (tkn.str == "Move") { // <K> -> Move Id Show Id
      node->ch1 = getLeafNode(tkn.str);
      tkn = scanner(*fp);
      if (tkn.id == ID_TK) {
        if (!verify(symbolTable, tkn)) error(tkn, ID_UNDEFINED); //
        node->ch2 = getDecoratedNode("Identifier", tkn);
        tkn = scanner(*fp);
        if (tkn.id == KEYWD_TK && tkn.str == "Show") {
          node->ch3 = getLeafNode(tkn.str);
          tkn = scanner(*fp);
          if (tkn.id == ID_TK) {
            if (!verify(symbolTable, tkn)) error(tkn, ID_UNDEFINED); //
            node->ch4 = getDecoratedNode("Identifier", tkn);
            tkn = scanner(*fp);
          } else error();
        } else error();
      } else error();
    } else error();
  } else error();
  
  return node;
}

// <L> -> Flip Id
node_t* L() {
  node_t* node = getNode("<L>");

  if (tkn.id == KEYWD_TK && tkn.str == "Flip") {
    node->ch1 = getLeafNode(tkn.str);
    tkn = scanner(*fp);
    if (tkn.id == ID_TK) {
      if (!verify(symbolTable, tkn)) error(tkn, ID_UNDEFINED); //
      node->ch2 = getDecoratedNode("Identifier", tkn);
      tkn = scanner(*fp);
    } else error();
  } else error();
  
  return node;
}

// <W> -> Num <V> Num | Num .
node_t* W() {
  node_t* node = getNode("<W>");

  if (tkn.id == NUM_TK) {
    node->ch1 = getDecoratedNode("Number", tkn);
    tkn = scanner(*fp);
    if (tkn.id == OP_TK) {
      if (tkn.str == ".") { // <W> -> Num .
        node->ch2 = getLeafNode(tkn.str);
        tkn = scanner(*fp);
      } else if (tkn.str == "+" || tkn.str == "%" || tkn.str == "&") { // <W> -> Num <V> Num
        node->ch2 = V();
        if (tkn.id == NUM_TK) {
          node->ch3 = getDecoratedNode("Number", tkn);
          tkn = scanner(*fp);
        } else error();
      } else error();
    } else error();
  } else error();
  
  return node;
}

// <Z> -> Id | Num
node_t* Z() {
  node_t* node = getNode("<Z>");
  
  if (tkn.id == ID_TK) { // <Z> -> Id
    if (!verify(symbolTable, tkn)) error(tkn, ID_UNDEFINED); //
    node->ch1 = getDecoratedNode("Identifier", tkn);
    tkn = scanner(*fp);
  } else if (tkn.id == NUM_TK) { // <Z> -> Num
    node->ch1 = getDecoratedNode("Number", tkn);
    tkn = scanner(*fp);
  } else error();
  
  return node; 
}
