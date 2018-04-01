#include <iostream>
#include <unistd.h>
#include <signal.h>

void mi_manejador(int senial) {
    std::cout<<"Me llego una alarma, mi pid "<< getpid()<<std::endl;
}


int main() {
    std::cout << "Hello, World!" << std::endl;

    struct sigaction act;
    act.sa_handler = mi_manejador;

    sigaction(SIGALRM, &act, NULL);

    pid_t pid = fork();

    if (pid==0) {
        std::cout << "Soy el hijo "<< getpid() << std::endl;
        sleep(10);
    } else {
        std::cout << "Soy el padre "<< getpid() << std::endl;
        alarm(5);
        std::cout << "Alarma asincronica en el padre mi pid: "<< getpid() << std::endl;
        sleep(15);
        std::cout << "Termina el padre "<< getpid() << std::endl;
    }

    return 0;
}