// Lexi Anderson
// CS4280, Proj 4
// generateCode.cpp

#include <climits>
#include <iostream>
#include <string>
#include <vector>

#include "generateCode.h"
#include "node.h"

FILE* asmFile;

std::vector<std::string> varNames;
std::vector<int> varVals;

int numTemps = 0;
int numLabels = 0;

int acc = 0;

// Add new variable to vector
// Return variable value
int newVar(std::string name, int val) {
  varNames.push_back(name);
  varVals.push_back(val);
  
  return val;
}

// Create temporary variable to store int value
// Return variable name
std::string newTemp(int val) {
  std::string varName = "T" + std::to_string(++numTemps);
  
  newVar(varName, val);
  
  return varName;
}

// Create new assembly label to use in generated code
std::string newLabel(std::string prefix) {
  std::string labelName = prefix + std::to_string(++numLabels);
  
  return labelName;
}

// Update variable value
// Return new variable value
int setVal(std::string name, int newVal) {
  int ind = getVarIndex(name);
  
  if (ind == -1) return ind;
  
  varVals[ind] = newVal;
  
  return newVal;
}

// Get current value of variable
int getVal(std::string name) {
  int ind = getVarIndex(name);
  
  if (ind == -1) return ind;
  else return varVals[ind];
}

// Get index of a variable stored in vector
int getVarIndex(std::string name) {
  auto it = std::find(varNames.begin(), varNames.end(), name);
  
  if (it == varNames.end()) return -1;
  else return it - varNames.begin();
}


// Write code to assembly file
void write(const char* fmt...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(asmFile, fmt, args);
  va_end(args);
}

void writeVars() {
  // TODO: implement
}

void genCode(node_t* root) {
  if (root == nullptr) return;
  
  if (root->label == "<S>") genS(root);
}

void genS(node_t* root) {
  std::string varName;

  if (root == nullptr) return;

  // Name Id
  varName = root->ch2->decor->str;
  acc = newVar(varName, 0);
  write("LOAD %d\n", acc);
  write("STORE %s\n", varName.c_str());

  // Spot Id
  varName = root->ch4->decor->str;
  newVar(varName, 0);  // value not yet read in from user
  write("READ %s\n", varName.c_str());
  
  genR(root->ch5);
  
  genE(root->ch6);
  
  write("STOP\n");
  writeVars();
}

void genR(node_t* root) {
  if (root == nullptr) return;

  // ignore Place
  
  genA(root->ch2);
  
  genB(root->ch3);
  
  // ignore Home
}

void genE(node_t* root) {
  std::string varName;

  if (root == nullptr) return;

  // Show Id
  varName = root->ch2->decor->str;
  write("WRITE %s\n", varName.c_str());
}

void genA(node_t* root) {
  std::string varName;

  if (root == nullptr) return;
  
  // Name Id
  varName = root->ch2->decor->str;
  acc = newVar(varName, 0);
  
  write("LOAD %d\n", acc);
  write("STORE %s\n", varName.c_str());
}

void genB(node_t* root) {
  if (root == nullptr) return;
  
  // empty
  if (root->ch1 == nullptr) return;
  
  if (root->ch1->label == ".") { // . <C> . <B>
    genC(root->ch2);
    genB(root->ch4);
  } else { // <D> <B>
    genD(root->ch1);
    genB(root->ch2);
  }
}

void genC(node_t* root) {
  if (root == nullptr) return;
  
  if (root->ch1->ch1->label == "{") genF(root->ch1);
  else genG(root->ch1);
}

// modifies vars
std::string genD(node_t* root) {
  std::string varName, nextToken;

  if (root == nullptr) return "";
  
  nextToken = root->ch1->ch1->label;
  
  if (nextToken == "/") varName = genH(root->ch1);
  else if (nextToken == "Assign") varName = genJ(root->ch1);
  else if (nextToken == "Spot" || nextLabel == "Move") varName = genK(root->ch1);
  else if (nextToken == "Flip") varName = genL(root->ch1);
  else if (nextToken == "Show") varName = genE(root->ch1);
  else varName = genF(root->ch1);
  
  return varName;
}

std::string genF(node_t* root) {
  std::string varNameX, varNameY, inLabel, outLabel, loopLabel;
  int x, y;

  if (root == nullptr) return "";
  
  if (root->ch2->label == "If") {
    varNameX = ch3->decor->str;
    x = getVal(varNameX);
    varNameY = genW(root->ch5);
    if (varNameY == "") exit(1);
    y = getVal(varNameY);
    
    inLabel = newLabel("In");
    outLabel = newLabel("Out");
    
    write("LOAD %d\n", y);
    write("SUB %s\n", varNameX.c_str());
  
    switch (root->ch4->label) {
      case "<<": // x < y
        write("BRPOS %s\n", inLabel.c_str());
        break;
      case "<-": // x >= y
        write("BRZNEG %s\n", inLabel.c_str());
        break;
    }
    
    write("BR %s\n", outLabel.c_str());
    write("%s: NOOP\n", inLabel.c_str());
    genD(root->ch6);
    write("BR %s\n", outLabel.c_str());
    write("%s: NOOP\n", outLabel.c_str());
  } else { // Do Again
    varNameX = genW(root->ch6);
    if (varNameX == "") exit(1);
    x = getVal(varNameX);
    
    loopLabel = newLabel("Loop");
    outLabel = newLabel("Out");
    
    write("%s: LOAD %s\n", loopLabel.c_str(), varNameX.c_str());
    
    switch (root->ch5->label) {
      case "<<":
        write("BRZPOS %s\n", outLabel.c_str());
        break;
      case "<-":
        write("BRNEG %s\n", outLabel.c_str());
        break;
    }
    
    genD(root->ch4);
    write("BR %s\n", loopLabel.c_str());
    write("%s: NOOP\n", outLabel.c_str());
  }
  
  return varName;
}

// resets acc
void genG(node_t* root) {
  std::string varName, inLabel, outLabel;

  if (root == nullptr) return;
  
  int x = stoi(root->ch2->decor->str);
  
  // store num in temp var
  varName = newTemp(x);
  inLabel = newLabel("In");
  outLabel = newLabel("Out");
  
  write("LOAD %s\n", varName.c_str());
  write("%s: WRITE %s\n", inLabel.c_str(), varName.c_str());
  write("SUB 1\n");
  write("BRZNEG %s\n", outLabel.c_str());
  write("BR %s\n", inLabel.c_str());
  write("%s: NOOP\n", outLabel.c_str());
  
  acc = 0;
}

// modifies variables
std::string genH(node_t* root) {
  std::string varName, nodeStr;
  node_t* z_prod;

  if (root == nullptr) return "";
  
  // traverse <Z>
  z_prod = root->ch2->ch1;
  
  if (z_prod->label == "Identifier") {
    varName = z_prod->decor->str;
  } else { // Number
    nodeStr = z_prod->decor->str;
    varName = newTemp(stoi(nodeStr));
  }
  
  write("LOAD %s\n", varName.c_str());
  write("SUB 1\n");
  write("STORE %s\n", varName.c_str());
  
  acc = setVal(varName, getVal(varName) - 1);
  
  return varName;
}

// modifies variables -- update vars vector and acc
std::string genJ(node_t* root) {
  std::string varNameX, varNameY;
  int x, y;
  
  if (root == nullptr) return "";
  
  // Assign Id
  varNameX = root->ch2->decor->str;
  x = getVal(varNameX);
  
  varNameY = genD(root->ch3);
  y = getVal(varNameY);
  
  write("LOAD %s\n", varNameY.c_str());
  write("STORE %s\n", varNameX.c_str());
  
  acc = setVal(varNameX, y);
  
  return varNameX;
}

// modifies vars
std::string genK(node_t* root) {
  std::string varName;
  int x;

  if (root == nullptr) return "";
  
  if (root->ch1->label == "Spot") { // Spot Num Show Num
    x = stoi(root->ch2->decor->str);
    varName = newTemp(x);
    
    write("LOAD %x\n", x);
    write("STORE %s\n", varName.c_str());
    write("WRITE %s\n", varName.c_str());
    
    acc = x;
  } else { // Move Id Show Id
    varName = newVar(root->ch2->decor->str);
    
    write("LOAD %s\n", varName.c_str());
    write("WRITE %s\n", varName.c_str());
    
    acc = getVal(varName);
  }
  
  return varName;
}

// modifies vars
std::string genL(node_t* root) {
  std::string varName;
  int x;
  
  if (root == nullptr) return "";
  
  // Flip Id
  varName = root->ch2->decor->str;
  x = getVal(varName);
  x = x * -1;
  
  write("LOAD %s\n", varName.c_str());
  write("MULT -1\n");
  write("STORE %s\n", varName.c_str());
  
  acc = setVal(varName, x);
  
  return varName;
}

// Returns name of temp variable storing integer value
std::string genW(node_t* root) {
  std::string varName, op;
  int x, y, result;
  
  if (root == nullptr) return "";
  
  if (root->ch3 != nullptr) { // Num <V> Num
    op = root->ch2->ch1->label;
    x = newTemp(stoi(root->ch1->decor->str));
    y = newTemp(stoi(root->ch3->decor->str));
    
    switch (op) {
      case "+":
        result = newTemp(x + y);
        break;
      case "%":
        result = newTemp(x / y);
        break;
      case "&":
        result = newTemp(x * y);
        break;
    }
    
    acc = result;
  } else { // Num .
    result = stoi(root->ch1->decor->str);
    acc = result;
  }
  
  varName = newTemp(result);
  return varName;
}
