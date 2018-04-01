#include <iostream>
#include <unistd.h>

int main() {
    std::cout << "Soy otro proceso" << getpid() << "Me duermo 10 seg"<< std::endl;
    sleep(10);
    return 0;
}