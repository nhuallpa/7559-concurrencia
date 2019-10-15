//
// Created by nestor on 01/09/19.
//

#ifndef SYMBOLTABLE_SYMBOLTABLE_H
#define SYMBOLTABLE_SYMBOLTABLE_H

#include "../Symbol/Symbol.h"
#include <vector>
#include <iostream>


using std::vector;
using std::cout;

class SymbolTable {
private:
    vector<Symbol> symbols;

public:

    void add(const Symbol &symbol);
    void printContent();

};


#endif //SYMBOLTABLE_SYMBOLTABLE_H
