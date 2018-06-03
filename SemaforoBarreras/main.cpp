#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <wait.h>
using namespace std;

void initSem(int idSem, int val) {
    union semun {
        int val;
        struct semid_is;
        unsigned short *array;
    };
    semun init;
    init.val = val;
    int code = semctl(idSem, 0, SETVAL, init);
    if (code == -1) {
        std::cout<<strerror(errno)<<std::endl;
    }
}



int getSem(const char * archivo, int valor) {
    key_t key = ftok(archivo, 'a');
    int idSem = semget(key, 1, 0777 | IPC_CREAT | IPC_EXCL);
    if (idSem == -1) {
        std::cout<<strerror(errno)<<std::endl;
    }
    initSem(idSem, valor);
    return idSem;
}

int mirandom() {
    return 1;
}

void p(int idSem){
    struct sembuf buf;
    buf.sem_op = -1;
    buf.sem_num = 0;
    buf.sem_flg = SEM_UNDO;
    semop(idSem, &buf, 1);
}

void v(int idSem){
    struct sembuf buf;
    buf.sem_op = 1;
    buf.sem_num = 0;
    buf.sem_flg = SEM_UNDO;
    semop(idSem, &buf, 1);
}

void barrera(int idSem){
    struct sembuf buf;
    buf.sem_op = 0;
    buf.sem_num = 0;
    buf.sem_flg = 0;
    semop(idSem, &buf, 1);
}

int deleteSem(int idSem) {
    return semctl(idSem, 0, IPC_RMID);
}

void calcularAtaque() {

}

int main() {
    int idAtaque = getSem("/bin/bash", 4);
    int idRonda = getSem("/bin/ls", 4);
    int idInicializacion = getSem("/bin/cat", 4);

    int cantRondas = 3;

    int i=1;
    for (i=1; i <= 4; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            for (int i=0 ; i<cantRondas; i++) {
                int valor = mirandom();


                p(idAtaque);
                barrera(idAtaque);
                cout<<getpid()<<":Ataquen"<<endl;
                initSem(idInicializacion, 4);

                calcularAtaque();

                p(idRonda);
                barrera(idRonda);
                cout<<getpid()<<":Siguiente Ronda"<<endl;


                initSem(idAtaque, 4);
                p(idInicializacion);
                barrera(idInicializacion);
                cout<<getpid()<<":Inicializamos"<<endl;
                initSem(idRonda, 4);

            }
            exit(0);
        }
    }
    int j;
    for (j=0; j<4; j++) {
        wait(NULL);
    }
    deleteSem(idAtaque);
    deleteSem(idRonda);
    deleteSem(idInicializacion);
    return 0;
}