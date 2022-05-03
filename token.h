// Lexi Anderson
// CS4280, Proj 4
// token.h

#ifndef TOKEN_H
#define TOKEN_H

#include <map>
#include <string>


enum tokenID {
  ID_TK = 1001,
  COMM_TK = 1002,
  NUM_TK = 1003,
  KEYWD_TK = 1004,
  OP_TK = 1005,
  EOF_TK = 1006,
  ERR_TK = -1
};

const std::map<tokenID, std::string> tokenNames = {
  { ID_TK, "Identifier" },
  { COMM_TK, "Comment" },
  { NUM_TK, "Number" },
  { KEYWD_TK, "Keyword" },
  { OP_TK, "Operator" },
  { EOF_TK, "EOF" },
  { ERR_TK, "Error" }
};

// Respective int values for each final state
// All final states < 0 or > 1000
const int finalStates[] = {
  1001,
  1002,
  1003,
  1004,
  1005,
  1006,
  -1
};

typedef struct token_t {
  tokenID id = ERR_TK;
  std::string str = "";
  int line = 0;
} token_t;

// Array of valid keywords
const std::string keywords[] = {
  "Again",
  "If", 
  "Assign", 
  "Move", 
  "Show", 
  "Flip", 
  "Name", 
  "Home", 
  "Do", 
  "Spot", 
  "Place", 
  "Here", 
  "There"
};

#endif
