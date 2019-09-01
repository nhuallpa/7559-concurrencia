#include <iostream>

extern char **environ;

int main(int argc, char *argv[], char *envp[]) {

    std::cout<<"Opcion 1: con variable global"<<std::endl;
    char **ep;
    for (ep = environ; *ep != NULL; ep++) {
        puts(*ep);
    }

    std::cout<<"=============================="<<std::endl;
    std::cout<<"Opcion 2: Con tercer parametro"<<std::endl;
    for (ep = envp; *ep != NULL; ep++) {
        puts(*ep);
    }

    return 0;
}