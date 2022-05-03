// Lexi Anderson
// CS4280, Proj 3
// main.cpp

#include <fstream>
#include <iostream>
#include <stdio.h>

#include "node.h"
#include "parser.h"

std::string fileName;

void printArgV(int, char*[]);


int main(int argc, char* argv[]) {
  std::istream* fp;
  
  if (argc == 0 || argc > 2) {
    printf("Fatal: Improper usage\nUsage:");
    printArgV(argc, argv);
    exit(1);
  }
  else if (argc == 2) fp = new std::ifstream(argv[1]);
  else if (argc == 1) fp = &std::cin;
  
  node_t* root = parser(*fp);
  
  // TODO: call static semantic function on the tree
  
  if (fp != &std::cin) delete fp;
  deleteTree(root);
  
  return 0;
}

// Print the entered command followed by line break
// argc -- argument count
// argv -- argument array
void printArgV(int argc, char* argv[]) {
  int i;
  for (i = 0; i < argc; i++) std::cout << " " << argv[i];
  std::cout << std::endl;
}
