#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>


long int rndom() {
    srand(getpid());
    long int result = rand() % 30;
    return result;
}

/**
 * Para pasar la informacion, el proceso hijo termina para poder comunicarse con el padre
 * Este mecanismo esta limitado por el tamaño de codigo de retorno.
 * */
int main(int argc, const char* argv[]) {
    std::cout<<"Comenzando"<<std::endl;
    int index = 0;
    int code = 0;
    int countChilders = 3;//atoi(argv[1]);
    int status = 0;
    pid_t pid = 0;

    // Creacion de una clave para identificar univocamante a la memoria.
    char proj_id = '8';
    key_t key = ftok("/bin/bash", proj_id);
    if (key<0) {
        std::cout << "Fallo ftok " << strerror(errno) << '\n';
    }
    // creación de memoria
    int shmid = shmget(key, countChilders * sizeof(int), 0644|IPC_CREAT);
    if (shmid<0) {
        std::cout << "Fallo shmget " << strerror(errno) << '\n';
    }
    // mapeo segmento a un espacio de memoria
    int* pMemCompartida = (int*)shmat(shmid, NULL, 0);

    if (pMemCompartida == (void *)-1) {
        std::cout << "Fallo shmat " << strerror(errno) << '\n';
    }
    std::cout << "Memoria asignada " << pMemCompartida <<std::endl;
    while (index < countChilders) {
        pid = fork();
        if (pid == 0) {

            int time = rndom();
            std::cout<< "Soy el hijo"<< getpid() << "acceder a"<< index << std::endl;
            pMemCompartida = pMemCompartida + index;
            *pMemCompartida = time;
            status = index;
            std::cout<<"Mi pid es "<< getpid() <<" me dieron el tiempo "<<time << " segundos."<<std::endl;
            sleep(time);
            break;
        }
        index++;
    }

    if (pid > 0) {
        index = 0;
        int status = 0;
        while (index < countChilders) {
            pid_t pidChild = wait(&status);

            int indiceHijo = WEXITSTATUS(status);
            int time = pMemCompartida[indiceHijo];

            std::cout<<"Soy el padre, el pid "<< pidChild << " termino en " << time << " segundos."<<std::endl;
            index++;
        }

        // detach de la memoria compartida
        code = shmdt(pMemCompartida);
        if (code<0) {
            std::cout << "Fallo shmdt " << strerror(errno) << '\n';
        }
        // destruccion de la memoria compartida.
        code = shmctl(shmid, IPC_RMID, NULL);
        if (code<0) {
            std::cout << "Fallo shmctl " << strerror(errno) << '\n';
        }
    }



    return status;
}
