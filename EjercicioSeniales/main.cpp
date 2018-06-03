#include <iostream>
#include <unistd.h>
#include <signal.h>

void mi_manejador(int senial) {

    sigset_t sigset;
    sigaddset(&sigset, SIGINT);
    sigprocmask(SIG_BLOCK, &sigset, NULL);
    std::cout<<"Me llego la señal pid "<< getpid()<<std::endl;
    sleep(10);
    std::cout<<"Termino manejador "<< getpid()<<std::endl;
}


int main() {
    std::cout << "Soy el padre" << getpid()<< std::endl;

    struct sigaction act;
    sigaddset(&act.sa_mask, SIGINT);
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