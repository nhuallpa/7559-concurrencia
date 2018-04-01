#include <iostream>
#include <unistd.h>
#include <signal.h>

void mi_manejador(int senial) {
    std::cout<<"Me llego la señal pid "<< getpid()<<std::endl;
}


int main() {
    std::cout << "Hello, World!" << std::endl;

    struct sigaction act;
    act.sa_handler = mi_manejador;

    sigaction(SIGINT, &act, NULL);

    pid_t pid = fork();

    if (pid==0) {
        std::cout << "Soy el hijo "<< getpid() << std::endl;
        sleep(10);
    } else {
        sleep(15);
        std::cout << "Termina el padre "<< getpid() << std::endl;
    }

    return 0;
}