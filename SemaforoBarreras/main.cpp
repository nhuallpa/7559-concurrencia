#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>

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



int getSem(const char * archivo, int valor, int proj) {
    key_t key = ftok(archivo, proj);
    int idSem = semget(key, 1, 0777 | IPC_CREAT | IPC_EXCL);
    if (idSem == -1) {
        std::cout<<strerror(errno)<<std::endl;
    }
    initSem(idSem, valor);
    return idSem;
}

int mirandom() {
    srand(time(NULL)+getpid());
    return rand() % 100 + 1;
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

void w(int idSem){
    struct sembuf buf;
    buf.sem_op = 0;
    buf.sem_num = 0;
    buf.sem_flg = 0;
    semop(idSem, &buf, 1);
}

int deleteSem(int idSem) {
    return semctl(idSem, 0, IPC_RMID);
}

int calcularAtaque(int* valores, int i) {
    int perdi=0;
    for (int k=0; k < 4; k++) {
        if (i != k && valores[i] < valores[k]) {
            perdi = 1;
            break;
        }
    }
    return perdi;
}

int getMem(const char * archivo, int valor, int proj) {
    int* valores;
    key_t key = ftok("/bin/mv", 0);
    int shmid = shmget(key, sizeof(int)*4, 0666|IPC_CREAT|IPC_EXCL);
    return shmid;
}

int main() {

    int shmid = getMem("/bin/mv", 4, 'c');
    int* valores = (int*)shmat(shmid, NULL, 0);

    int idAtaque = getSem("/bin/bash", 4, 'a');
    int idRonda = getSem("/bin/ls", 0, 'b');
    int idReset = getSem("/bin/cp", 4, 'b');
    int cantRondas = 100;

    int i=0;
    for (i=0; i < 4; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            int perdi = 0;
            for (int j=0 ; j<cantRondas; j++) {

                //valores[i] = (perdi)?0:mirandom();
                valores[i] = mirandom();

                p(idAtaque);
                w(idAtaque);
                v(idRonda);
                cout<<getpid()<<":Ataco con: "<<valores[i]<<endl;
                perdi = calcularAtaque(valores, i);
                /*if (perdi) {
                    cout<<getpid()<<":No juega mas"<<endl;
                }*/

                p(idReset);
                w(idReset);
                v(idAtaque);

                p(idRonda);
                w(idRonda);
                v(idReset);
                cout<<getpid()<<":Siguiente Ronda"<<endl;

            }
            if (shmdt(valores) == -1) {
                cout<<strerror(errno)<<endl;
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
    deleteSem(idReset);

    if (shmdt(valores) == -1) {
        cout<<strerror(errno)<<endl;
    }

    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}