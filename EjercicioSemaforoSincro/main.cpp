#include <iostream>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <cstring>

using std::string;

int getSem(string path, int valor);
void waitSem(int semid);
void signalSem(int semid);


int main() {

    int semPadre = getSem("/bin/ls", 1);
    int semHijo = getSem("/bin/mv", 0);
    pid_t pid = fork();
    if (pid) { // padre
        for (int i = 0; i < 10; ++i) {
            waitSem(semPadre);
            std::cout << "Operacion del padre" << std::endl;
            signalSem(semHijo);
            sleep(1);
        }
    } else { // hijo
        for (int i=0; i <10; ++i) {
            waitSem(semHijo);
            std::cout << "Operacion del hijo" << std::endl;
            signalSem(semPadre);
            sleep(1);
        }
        exit(0);
    }

    int status = semctl(semPadre,0,IPC_RMID);
    int status = semctl(semHijo,0,IPC_RMID);

    std::cout << "Final" << std::endl;
    return 0;
}


int getSem(string path, int valor) {
    key_t key = ftok(path.c_str(), 6);
    int idsem = semget(key, 1, 0666 | IPC_CREAT);
    if (idsem != -1) {
        union semun {
            int val;
            struct semid_ds *buf;
            unsigned short *array;
            struct seminfo *__buff;
        };

        semun init;
        init.val = valor;
        int status = semctl(idsem, 0, SETVAL, init);
        if (status == -1) {std::cout<<"error semctl:"<<strerror(errno)<<std::endl;}
    } else {
        std::cout<<"error semget:"<<strerror(errno)<<std::endl;
    }
    return idsem;
}

void waitSem(int semid) {

    struct sembuf oper;
    oper.sem_num = 0;
    oper.sem_op = -1;
    oper.sem_flg = SEM_UNDO;

    int status = semop(semid, &oper, 1);
    if (status == -1) {std::cout<<"error wait:"<<strerror(errno)<<std::endl;}
}

void signalSem(int semid) {

    struct sembuf oper;
    oper.sem_num = 0;
    oper.sem_op = 1;
    oper.sem_flg = SEM_UNDO;

    int status = semop(semid, &oper, 1);
    if (status == -1) {std::cout<<"error signal:"<<strerror(errno)<<std::endl;}

}