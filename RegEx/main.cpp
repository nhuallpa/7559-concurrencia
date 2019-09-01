// regex_match example
#include <iostream>
#include <string>
#include <regex>


int main ()
{

    const char cstr[] = "insertar(nombre,dire,telefono)";

    std::regex e ("(insertar)\\((.*),(.*),(.*)\\)");

    std::cmatch cm;    // same as std::match_results<const char*> cm;
    if (std::regex_match (cstr,cm,e))
        std::cout << "string literal with " << cm.size() << " matches\n";

    std::cout << "the matches were: ";
    for (unsigned i=0; i<cm.size(); ++i) {
        std::cout << "[" << cm[i] << "] ";
    }

    std::cout << std::endl;

    return 0;
}