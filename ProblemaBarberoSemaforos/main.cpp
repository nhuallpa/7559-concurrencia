#include <iostream>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>


using namespace std;

union semnum{
    int val;
    struct semid_ds* buf;
    ushort* array;
};


int getSem(const char* path, int val, char id);

void p(int idSem);

void v(int idSem);

int main() {
    std::cout << "Problema del barbero" << std::endl;
    int idSemHayBarbero = getSem("/bin/bash",1, 'a');
    int idSemHayCliente = getSem("/bin/ls",0, 'b');
    std::cout << "Sem barbero "<<idSemHayBarbero << std::endl;
    std::cout << "Sem Cliente "<<idSemHayCliente << std::endl;
    int hijo[11];
    pid_t pid = fork();

    if (pid == 0) {

        for (int i = 0; i < 10; i++) {
            std::cout << "Hay Clientes?" << std::endl;
            p(idSemHayCliente);
            std::cout << "Barbero Cortando Pelo" << std::endl;
            v(idSemHayBarbero);
            std::cout << "Barbero Termino" << std::endl;
        }

        exit(0);
    } else {
        hijo[0] = pid;
        for (int i = 0; i < 10; i++) {
            pid_t pidCli = fork();
            if (pidCli == 0) {
                std::cout<<"Cliente ("<<getpid()<<"): Soy cliente nuevo y espero"<<endl;
                v(idSemHayCliente);
                p(idSemHayBarbero);
                std::cout<<"Cliente ("<<getpid()<<"): Al fin me atienden"<<endl;

                exit(0);
            } else {
                hijo[i+1] = pidCli;
            }
        }
    }

    for (int j = 0; j < 11; ++j) {
        int status;
        wait(&status);
    }

    std::cout << "Elimino semaforo" << std::endl;
    semctl(idSemHayCliente, 0, IPC_RMID);
    semctl(idSemHayBarbero, 0, IPC_RMID);

    return 0;
}

void v(int idSem) {
    struct sembuf operacion2;
    operacion2.sem_num = 0;
    operacion2.sem_op = 1;         // decremente 1 ; v ; signal
    operacion2.sem_flg = SEM_UNDO;
    int estado = semop (idSem, &operacion2, 1);
    if (estado == -1) {
        std::cout<<getpid()<<"error v semop"<<endl;
    }
}

void p(int idSem) {
    struct sembuf operacion;
    operacion.sem_num = 0;
    operacion.sem_op = -1;         // decremente -1 ; p ; wait
    operacion.sem_flg = SEM_UNDO;
    int estado = semop (idSem, &operacion, 1);
    if (estado == -1) {
        std::cout<<getpid()<<"error p semop"<<endl;
    }
}

int getSem(const char *path, int val, char id) {// creo identificador del conjunto de semaforos.
    key_t key = ftok(path, 'a');
    int idSem = semget(key, 1,0666|IPC_CREAT);
    if (idSem != -1) {
        //inicializo semaforo
        semnum init;
        init.val=val;      // valor inicial del semaforo
        int estado = semctl(idSem, 0, SETVAL, init);
        if (estado == -1) {
            std::cout<<"error smctl"<<endl;
        }
    } else {
        std::cout<<"error idsem"<<endl;
    }

    return idSem;
}