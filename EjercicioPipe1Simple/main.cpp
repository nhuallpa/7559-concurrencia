#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <cstring>
#include <sstream>
#include <sys/wait.h>

const int TAMBUFFER = 100;

int main() {


    int pipefd[2];
    int status = pipe(pipefd);
    if (status == -1) {
        std::cerr<<"Error " << strerror(errno);
    } else {
        pid_t pid = fork();
        if (pid == 0) {
            std::cout << "Soy el hijo" << std::endl;
            close(pipefd[0]); // cierro el pipe de lectura

            char mensaje[TAMBUFFER] = "Hola Pipe";
            std::cout << "y envio: " <<mensaje<< std::endl;

            ssize_t retorno = write(pipefd[1], mensaje, TAMBUFFER);
            if (retorno == -1) {
                std::cerr<<"Error " << strerror(errno);
            }
            close(pipefd[1]); // cierro salida
        } else {
            close(pipefd[1]); // cierro el pipe de escritura

            char buffer[100];

            ssize_t retorno = read(pipefd[0], static_cast < void * >(buffer), 100);
            if (retorno == -1) {
                std::cerr<<"Error " << strerror(errno);
            } else {
                std::cout << "Soy el Padre" << std::endl;
                std::cout<<"Mensaje recido: "<< buffer <<std::endl;
            }

            close(pipefd[0]);  // cierro el byte de lectura
            wait(NULL);
            exit(0);
        }
    }


    return 0;
}