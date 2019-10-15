//
// Created by nestor on 01/09/19.
//

#include "Symbol.h"
using std::string;

Symbol::Symbol() {}
Symbol::~Symbol() {}

Symbol::Symbol(int indice, const string &symbol) : indice(indice), symbol(symbol) {}

bool Symbol::operator==(const Symbol &rhs) const {
    return indice == rhs.indice &&
           symbol == rhs.symbol;
}

bool Symbol::operator!=(const Symbol &rhs) const {
    return !(rhs == *this);
}

int Symbol::getIndice() const {
    return indice;
}

void Symbol::setIndice(int indice) {
    Symbol::indice = indice;
}

const string &Symbol::getSymbol() const {
    return symbol;
}

void Symbol::setSymbol(const string &symbol) {
    Symbol::symbol = symbol;
}

