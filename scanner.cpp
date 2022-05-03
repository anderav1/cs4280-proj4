// Lexi Anderson
// CS4280, Proj 3
// scanner.cpp

#include <ctype.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include "scanner.h"
#include "token.h"


// 2D array representation of the FSA
const int fsaTable[10][15] = {
          /* A-Z  a-z  0-9   *   WS    &    +    /    %    .    {    }    <    -  EOF */
/* S0 */  {   6,   1,   5,   3,   0,   8,   8,   8,   8,   8,   8,   8,   9,  -1,1006 },
/* S1 */  {   2,   2,   2,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1 },
/* S2 */  {   2,   2,   2,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001 },
/* S3 */  {   3,   3,   3,   4,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3 },
/* S4 */  {1002,1002,1002,1002,1002,1002,1002,1002,1002,1002,1002,1002,1002,1002,1002 },
/* S5 */  {1003,1003,   5,1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,1003 },
/* S6 */  {  -1,   7,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1 },
/* S7 */  {1004,   7,1004,1004,1004,1004,1004,1004,1004,1004,1004,1004,1004,1004,1004 },
/* S8 */  {1005,1005,1005,1005,1005,1005,1005,1005,1005,1005,1005,1005,1005,1005,1005 },
/* S9 */  {  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,   8,   8,  -1 },
};

int line = 1;
//static int line = 1;
std::string prefix = "";

// Scan and return the next token
// input -- input stream (either file or cin)
token_t scanner(std::istream &input) {
  token_t tkn;

  int state = 0;
  int nextState = 0;
  
  std::string str = prefix + "";
  char ch;
  
  int col;  // FSA table column
  
  // process chars until final state
  while (state >= 0 && state < 1000) {
    input.get(ch);    
    col = getTableCol(ch);
    
    if (input.eof()) col = 14;
    
    if (col == -1) {  // invalid char
      tkn.id = ERR_TK;
      tkn.str = str + ch;
      tkn.line = line;
      
      return tkn;
    }
    
    nextState = fsaTable[state][col];
    
    if (nextState == 1002) { // comment token; consume
      str = "";
      nextState = 0;
      state = 0;
    } else if (nextState == 1004) { // keyword token; validate
      bool valid = false;
      
      for (const std::string& keywd : keywords) {
        if (keywd.compare(str) == 0) {
          valid = true;
          break;
        }
      }
      
      if (!valid) {
        tkn.id = ERR_TK;
        tkn.str = str;
        tkn.line = line;
        
        std::cout << tkn.str << " is invalid keyword" << std::endl;
        
        nextState = -1;
        state = -1;
        
        return tkn;
      }
    }
    
    if (nextState >= 1000 || nextState < 0) { // final state, current char is lookahead
      tkn.id = (tokenID)nextState;
      
      if (nextState == 1006) {
        tkn.str = "EOF";
      } else tkn.str = str;
      
      tkn.line = line;
      
      //if (nextState >= 1000 && !isspace(ch)) str = ch;
      
      //std::cout << "Scanned \'" << tkn.str << "\' on line " << tkn.line << std::endl;
      
      return tkn;
    }
    
    if(!isspace(ch)) str += ch;
    state = nextState;
    if (ch == '\n') line++;
  }
  
  // beyond this results in error
  
  tkn.id = ERR_TK;
  tkn.str = "Scanning error";
  tkn.line = line;
  
  return tkn;
}

// Return the index of the column in the FSA table corresponding to the character ch
// ch -- character to match
int getTableCol(char ch) {
  if (isupper(ch) > 0) return 0;
  if (islower(ch) > 0) return 1;
  if (isdigit(ch) > 0) return 2;
  if (ch == '*') return 3;
  if (isspace(ch) > 0) return 4;
  
  if (ch == '&') return 5;
  if (ch == '+') return 6;
  if (ch == '/') return 7;
  if (ch == '%') return 8;
  if (ch == '.') return 9;
  if (ch == '{') return 10;
  if (ch == '}') return 11;
  if (ch == '<') return 12;
  if (ch == '-') return 13;
  
  if (ch == EOF) return 14;
  
  return -1;
}
