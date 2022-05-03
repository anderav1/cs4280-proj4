// Lexi Anderson
// CS4280, Proj 3
// semantics.cpp

#include <iostream>

#include "list.h"
#include "semantics.h"
#include "token.h"


// Insert the identifier if not already in the symbol table
// Return error if already in symbol table
void insert(list_t* symbolTable, token_t tkn) {
  if (verify(symbolTable, tkn)) error(tkn, ID_REDEFINED);
  else addToList(symbolTable, tkn);
}

// Check if identifier is already in symbol table
// Return true if token is already in list
bool verify(list_t* symbolTable, token_t tkn) {
  return isInList(symbolTable, tkn);
}

// Print error message and exit
void error(token_t tkn, semError err) {
  std::cout << "Error: Line " << tkn.line << ": Identifier \'" << tkn.str << "\'";

  switch(err) {
    case ID_REDEFINED:
      std::cout << " has already been defined" << std::endl;
      break;
    
    case ID_UNDEFINED:
      std::cout << " has not been defined" << std::endl;
      break;
  }
  
  exit(0);
}
