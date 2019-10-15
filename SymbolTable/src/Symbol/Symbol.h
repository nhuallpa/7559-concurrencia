//
// Created by nestor on 01/09/19.
//

#ifndef SYMBOLTABLE_SYMBOL_H
#define SYMBOLTABLE_SYMBOL_H

#include <string>

using std::string;
class Symbol {

private:
    int indice;
    string symbol;
public:
    Symbol();
    ~Symbol();

    Symbol(int indice, const string &symbol);

    bool operator==(const Symbol &rhs) const;

    bool operator!=(const Symbol &rhs) const;

    int getIndice() const ;

    void setIndice(int indice);

    const string &getSymbol() const;

    void setSymbol(const string &symbol);

};


#endif //SYMBOLTABLE_SYMBOL_H
