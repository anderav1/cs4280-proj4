// Lexi Anderson
// CS4280, Proj 3
// semantics.h

#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "list.h"
#include "token.h"

enum semError {
  ID_REDEFINED,
  ID_UNDEFINED
};

void insert(list_t*, token_t);
bool verify(list_t*, token_t);
void error(token_t, semError);


#endif
