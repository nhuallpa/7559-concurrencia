//
// Created by nestor on 01/09/19.
//

#include "SymbolTable.h"
using std::cout;
using std::string;
using std::endl;

void SymbolTable::add(const Symbol & symbol) {
    this->symbols.push_back(symbol);
}

void SymbolTable::printContent() {

    for ( auto symbol = this->symbols.begin(); symbol != this->symbols.end(); ++symbol) {
        cout << (*symbol).getSymbol() << endl;
    }
}
