#include <iostream>
#include <malloc.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>


/*
* Inicializa semaforos de un conjunto con un valor "n" tomado de consola.
**/
int initsem ( int semid ,int nbsem ) {
    unsigned short * op ;
    int i , n;
    op = ( unsigned short *) malloc ( sizeof ( unsigned short )* nbsem );

    for ( i = 0; i < nbsem ;i ++ ) {
        printf("sem[%d] = ", i);
        scanf("%d", &n);
        op[i] = n;
    }
    return semctl(semid , nbsem , SETALL , op );
}

/*
* RTA: Muestra el valor del semaforo indicado por "nbsem" en el conjunto identifcado por "semid"
**/
int mostrar ( int semid ,int nbsem ) {
    int i, val;
    for (i = 0; i < nbsem; i++) {
        if ((val = semctl(semid, i, GETVAL)) == -1) {
            fprintf(stderr, " semaforo %d - > ", i);
            perror(" semctl ");
        } else
            printf("semaforo %d = %d\n", i, val);
    }
    return 0;

}
int main() {
    std::cout << "Hello, World!" << std::endl;


    key_t key = ftok("/bin/bash", 'a');
    int cantidadSem = 2;
    int id = semget(key, cantidadSem, 0666| IPC_CREAT);
    if (id != -1) {
        int status = initsem(id, cantidadSem);
        if (status == -1) {
            std::cout<<"Error intsem"<<std::endl;
        }
    }

    mostrar(id,cantidadSem);

    int status = semctl(id,0,IPC_RMID);
    if (status == -1) {
        std::cout<<"Error eliminando semaforo"<<std::endl;
    }
    return 0;
}