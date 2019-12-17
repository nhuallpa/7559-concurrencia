#include <iostream>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
void static manejarSigint(int event) ;

/****
 * Los procesos el con mismo proceso padre tienen el mismo id de grupo.
 * EL kernel utiliza el process group id para identificar el grupo de procesos
 * relacionados que deben recibir señales comunes para ciertos eventos.
 *
 * En este caso el proceso HIJO no recive la señal porque se modifico el id del grupo
 * con setpgrp()
 *
 * @return
 */

int main () {
    int i ;
    signal ( SIGINT , manejarSigint ) ;
    if ( fork () == 0 )
        setpgrp(); // modifica el id grupo de proceso con el mismo pid del mismo (en el hijo)
    printf("el proceso de PID %d y PGRP %d esta esperando \n " , getpid () , getpgrp() ) ;
    for ( i = 1; i <= 3; i ++ ) {
        printf("El proceso %d sigue vivo \n " , getpid () ) ;
        sleep ( 1 ) ;
    }

    return 0;
}

void manejarSigint (int event) {
    printf("el proceso %d obtuvo un SIGINT en el grupo %d \n " , getpid (), getpgrp() ) ;
    exit ( 1 ) ;
}