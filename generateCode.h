// Lexi Anderson
// CS4280, Proj 4
// generateCode.h

#ifndef GENERATE_CODE_H
#define GENERATE_CODE_H

#include "node.h"

int newVar(std::string, int);
std::string newTemp(int);
std::string newLabel(std::string);
int setVal(std::string, int);
int getVal(std::string);
int getVarIndex(std::string);

void write(const char* ...);
void writeVars();

// Code generation functions
// may involve modification of values that need to be returned
// return values are names of variables accessed
void genCode(node_t*);
void genS(node_t*);
void genR(node_t*);
std::string genE(node_t*);
std::string genA(node_t*);
void genB(node_t*);
void genC(node_t*);
std::string genD(node_t*);
std::string genF(node_t*);
void genG(node_t*);
std::string genH(node_t*);
std::string genJ(node_t*);
std::string genK(node_t*);
std::string genL(node_t*);
std::string genW(node_t*);


#endif
