#include <iostream>
#include <sys/types.h>
#include <sys/unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
int producir();

void consumir(int valor);

int getMemId(const char * path, int N);

int *attachMem(int idMemBuffer);

using namespace std;

union semnum{
    int val;
    struct semid_ds* buf;
    ushort* array;
};


int getSem(const char* path, int val);

void p(int idSem);

void v(int idSem);

int main() {

    int N=5;
    std::cout << "Productores consumidores" << std::endl;

    // creacion memoria compartida BUFFER
    int idMemBuffer = getMemId("/bin/bash", N);
    int* buffer = attachMem(idMemBuffer);

    // indice In_Ptr
    int idMemInPtr = getMemId("/bin/ls", 1);
    int* inPtr = attachMem(idMemInPtr);

    // indice Out_Ptr
    int idMemOutPtr = getMemId("/bin/cp", 1);
    int* outPtr = attachMem(idMemOutPtr);

    //inicializamos los indices
    *inPtr = 0;
    *outPtr = 0;

    int hayLugar = getSem("/bin/mv", N);
    int hayElementos = getSem("/bin/cat", 0);

    pid_t pid = fork();
    if (pid == 0) {

        for (int i = 0; i < 20; ++i) {

            p(hayElementos);
            int valor = buffer[*outPtr];
            *outPtr = (*outPtr + 1) % N;
            v(hayLugar);

            consumir(valor);
        }
    } else {

        for (int i = 0; i < 20; ++i) {
            int valor = producir();
            p(hayLugar);
            std::cout<<"Productor: "<<valor<<std::endl;
            buffer[*inPtr] = valor;
            *inPtr = (*inPtr + 1) % N;
            v(hayElementos);
        }

        wait(NULL);
        shmdt(buffer);
        shmctl(idMemBuffer, IPC_RMID, 0);
        shmdt(inPtr);
        shmctl(idMemInPtr, IPC_RMID, 0);
        shmdt(outPtr);
        shmctl(idMemOutPtr, IPC_RMID, 0);

        semctl(hayElementos, 0, IPC_RMID);
        semctl(hayLugar, 0, IPC_RMID);
    }



    return 0;

}

int *attachMem(int idMemBuffer) {
    int* buffer = (int*)shmat(idMemBuffer, NULL, 0);
    if (buffer == (void *)-1) {
        std::cout << "error en shmat" << std::endl;
    }
    return buffer;
}

int getMemId(const char * path, int N) {
    key_t key = ftok(path, 'a');
    int idMemBuffer = shmget(key, sizeof(int) * N, 0666 | IPC_CREAT);
    if (idMemBuffer == -1) {
        std::cout << "error en shmget" << std::endl;
    }
    return idMemBuffer;
}

void consumir(int valor) {
    std::cout << "Consumir: "<< valor << std::endl;
}

int producir() {
    return  rand() % 100 + 1;
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

int getSem(const char *path, int val) {// creo identificador del conjunto de semaforos.
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