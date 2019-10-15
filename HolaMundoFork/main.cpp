#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
int main() {

    int status = 0;
    pid_t pid = fork();

    if (pid == 0) {
        std::cout << "Hello, World! I am a child" << std::endl;
        std::cout << "Hello, World! I am a child" << std::endl;
        std::cout << "Hello, World! I am a child" << std::endl;
        std::cout << "Hello, World! I am a child" << std::endl;
    } else {
        std::cout << "Hello, World! I am a father" << std::endl;
        std::cout << "esperando" << std::endl;
        wait(&status);
        std::cout << "Terminamos" << std::endl;
    }
    return 0;
}