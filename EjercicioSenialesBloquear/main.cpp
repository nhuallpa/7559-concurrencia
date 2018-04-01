#include <iostream>
#include <unistd.h>
#include <signal.h>

int main() {
    std::cout << "Hello, World!" << std::endl;

    sigset_t sigset;

    sigaddset(&sigset, SIGINT);
    sigprocmask(SIG_BLOCK, &sigset, NULL);
    std::cout << "Soy el padre "<< getpid() << " Duermo 10 seg, no molesten"<< std::endl;
    sleep(10);
    std::cout << "Ya me levante"<< std::endl;
    return 0;
}
