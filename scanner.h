// Lexi Anderson
// CS4280, Proj 3
// scanner.h

#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>

#include "token.h"

token_t scanner(std::istream &input);
int getTableCol(char ch);


#endif
