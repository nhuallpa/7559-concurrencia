#include <iostream>
#include "src/Symbol/Symbol.h"
#include "src/SymbolTable/SymbolTable.h"

int main() {
    std::cout << "Listado de simbolos" << std::endl;

    SymbolTable tabla;
    Symbol symbol(1, "/");
    Symbol symbol2(2, "-");
    Symbol symbol3(3, ":");

    tabla.add(symbol);
    tabla.add(symbol2);
    tabla.add(symbol3);

    tabla.printContent();

    return 0;
}