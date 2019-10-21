#include <iostream>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <cstring>
#include <vector>

using std::string;
using std::vector;

int getSem(string path, int valor);
int getMemInt(string path);
int* atacharInt(int idMem);
void waitSem(int semid);
void signalSem(int semid);
int deleteMemInt(int idMem);
int detacharInt(int* idMem);
int deleteSem(int semid);

void intentoUnoStarvation();

void intentoDosAusenciaStarvation();

void intentoTresPrioridadEscritores();

void esperarProcesosHijos(vector<int> &pidsHijos);

const int CANT_LECTORES = 4;
const int CATN_ESCRITORES = 4;


int main() {

    std::cout << "Intento 1 : Hay starvation de escritores. " << getpid()<< std::endl;
    intentoUnoStarvation();

    std::cout << "Intento 2 : Ausencia de stavation - Lectores escritores. " << getpid()<< std::endl;
    intentoDosAusenciaStarvation();

    std::cout << "Intento 3 : Ausencia de stavation - Prioridad escritores. " << getpid()<< std::endl;
    intentoTresPrioridadEscritores();

    return 0;
}

void intentoTresPrioridadEscritores() {

    vector<int> pidsHijos;

    int idMem = getMemInt("/bin/ls");
    int idMemEscritores = getMemInt("/bin/sleep");
    int mutexL = getSem("/bin/ln", 1);
    int mutexE = getSem("/bin/cat", 1);
    int noWriters = getSem("/bin/cp", 1);
    int noReaders = getSem("/bin/mv", 1);
    int* lectoresIniciales = atacharInt(idMem);
    int* escritoresIniciales = atacharInt(idMemEscritores);
    *lectoresIniciales = 0;
    *escritoresIniciales = 0;

// Lectores
    for (int i=0; i<CANT_LECTORES; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            int* cantLectores = atacharInt(idMem);
            for (int j = 0; j < 10; ++j) {


                waitSem(noReaders);
                    waitSem(mutexL);
                    *cantLectores+=1;
                    if (*cantLectores == 1) waitSem(noWriters);
                    signalSem(mutexL);
                signalSem(noReaders);

                // Si el lector esta en la SC, este mantiene noWriters pero no mantiene noReaders
                std::cout << "Leer desde el proceso " << getpid()<< std::endl;

                waitSem(mutexL);
                *cantLectores-=1;
                if (*cantLectores == 0) signalSem(noWriters);
                signalSem(mutexL);
            }

            detacharInt(cantLectores);
            exit(0);
        } else {
            pidsHijos.push_back(pid);
        }
    }

    // Escritores
    for (int i=0; i<CATN_ESCRITORES; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            int* cantEscritores = atacharInt(idMemEscritores);

            for (int j = 0; j < 10; ++j) {
                waitSem(mutexE);
                *cantEscritores +=1;
                if (*cantEscritores == 1) waitSem(noReaders);
                signalSem(mutexE);

                waitSem(noWriters);
                // Cuando un escritor esta en SC, mantiene ambos semaforos, noWriters y noReaders
                std::cout << "Escribir desde el proceso " << getpid()<< std::endl;
                signalSem(noWriters);

                waitSem(mutexE);
                *cantEscritores -=1;
                if (*cantEscritores == 0) signalSem(noReaders);
                signalSem(mutexE);
            }

            exit(0);
        } else {
            pidsHijos.push_back(pid);
        }
    }


    esperarProcesosHijos(pidsHijos);


    detacharInt(lectoresIniciales);
    detacharInt(escritoresIniciales);
    deleteMemInt(idMem);
    deleteMemInt(idMemEscritores);
    deleteSem(mutexE);
    deleteSem(mutexL);
    deleteSem(noReaders);
    deleteSem(noWriters);

}


void intentoDosAusenciaStarvation() {

    vector<int> pidsHijos;

    int idMem = getMemInt("/bin/ls");
    int mutex = getSem("/bin/vm", 1);
    int roomEmpty = getSem("/bin/cp", 1);
    int turnsline = getSem("/bin/mv", 1);
    int* lectoresIniciales = atacharInt(idMem);
    *lectoresIniciales = 0;

    // Lectores
    for (int i=0; i<CANT_LECTORES; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            int* cantLectores = atacharInt(idMem);
            for (int j = 0; j < 10; ++j) {

                waitSem(turnsline);
                signalSem(turnsline);

                waitSem(mutex);
                *cantLectores+=1;
                if (*cantLectores == 1) waitSem(roomEmpty);
                signalSem(mutex);

                std::cout << "Leer desde el proceso " << getpid()<< std::endl;

                waitSem(mutex);
                *cantLectores-=1;
                if (*cantLectores == 0) signalSem(roomEmpty);
                signalSem(mutex);
            }

            detacharInt(cantLectores);
            exit(0);
        } else {
            pidsHijos.push_back(pid);
        }
    }

    // Escritores
    for (int i=0; i<CATN_ESCRITORES; ++i) {
        pid_t pid = fork();
        if (pid == 0) {

            for (int j = 0; j < 10; ++j) {
                waitSem(turnsline);
                waitSem(roomEmpty);

                std::cout << "Escribir desde el proceso " << getpid()<< std::endl;

                signalSem(turnsline);
                signalSem(roomEmpty);
            }

            exit(0);
        } else {
            pidsHijos.push_back(pid);
        }
    }

    esperarProcesosHijos(pidsHijos);


    detacharInt(lectoresIniciales);
    deleteMemInt(idMem);
    deleteSem(roomEmpty);
    deleteSem(mutex);
    deleteSem(turnsline);

}

void esperarProcesosHijos(vector<int> &pidsHijos) {
    for (vector<int>::iterator it = pidsHijos.begin(); it != pidsHijos.end(); ++it) {
        int wsstatus;
        waitpid(*it, &wsstatus, WUNTRACED | WCONTINUED);
    }
}

void intentoUnoStarvation() {

    vector<int> pidsHijos;

    int idMem = getMemInt("/bin/ls");
    int mutex = getSem("/bin/vm", 1);
    int roomEmpty = getSem("/bin/cp", 1);
    int* lectoresIniciales = atacharInt(idMem);
    *lectoresIniciales = 0;

    // Lectores
    for (int i=0; i<CANT_LECTORES; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            int* cantLectores = atacharInt(idMem);
            for (int j = 0; j < 10; ++j) {

                waitSem(mutex);
                *cantLectores+=1;
                if (*cantLectores == 1) waitSem(roomEmpty);
                signalSem(mutex);

                std::cout << "Leer desde el proceso " << getpid()<< std::endl;

                waitSem(mutex);
                *cantLectores-=1;
                if (*cantLectores == 0) signalSem(roomEmpty);
                signalSem(mutex);
            }

            detacharInt(cantLectores);
            exit(0);
        } else {
            pidsHijos.push_back(pid);
        }
    }

    // Escritores
    for (int i=0; i<CATN_ESCRITORES; ++i) {
        pid_t pid = fork();
        if (pid == 0) {

            for (int j = 0; j < 10; ++j) {
                waitSem(roomEmpty);
                std::cout << "Escribir desde el proceso " << getpid()<< std::endl;
                signalSem(roomEmpty);
            }

            exit(0);
        } else {
            pidsHijos.push_back(pid);
        }
    }

    esperarProcesosHijos(pidsHijos);

    detacharInt(lectoresIniciales);
    deleteMemInt(idMem);
    deleteSem(roomEmpty);
    deleteSem(mutex);
}

int getMemInt(string path) {
    key_t key = ftok(path.c_str(), 'a');
    int memId = shmget(key, sizeof(int), IPC_CREAT | 0644);
    if (memId == -1) {
        std::cout<<"error shmget:"<<strerror(errno)<<std::endl;
    }
    return memId;
}

int* atacharInt(int idMem) {
    void* mem = shmat(idMem, NULL, 0);
    if (mem == (void *) -1) {
        std::cout<<"error shmat:"<<strerror(errno)<<std::endl;
    }
    return (int *) mem;
}

int detacharInt(int* mem) {
    int status = shmdt((void *) mem);
    if (status == -1) {
        std::cout<<"error shmdt:"<<strerror(errno)<<std::endl;
    }
    return status;
}

int deleteMemInt(int idMem) {
    int status = shmctl(idMem, IPC_RMID, NULL);
    if (status == -1) {
        std::cout<<"error shmctl:"<<strerror(errno)<<std::endl;
    }
    return status;
}

int deleteSem(int semid) {
    int status = semctl(semid, 0, IPC_RMID);
    if (status == -1) {
        std::cout<<"error semctl:"<<strerror(errno)<<std::endl;
    }
    return status;
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