#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <sys/wait.h>

#include "Pipes/Pipe.h"


int main () {

	static const int BUFFSIZE = 100;

	Pipe canal;
	int pid = fork ();

	if ( pid == 0 ) {

		// lector
		char buffer[BUFFSIZE];

		std::cout << "Lector: esperando para leer..." << std::endl;
		ssize_t bytesLeidos = canal.leer ( static_cast<void*>(buffer),BUFFSIZE );
		std::string mensaje = buffer;
		mensaje.resize ( bytesLeidos );

        std::cout << "Lector: lei el dato [" << mensaje << "] (" << bytesLeidos << " bytes) del pipe" << std::endl;
        bytesLeidos = canal.leer ( static_cast<void*>(buffer),BUFFSIZE );
        mensaje = buffer;
        std::cout << "Lector: lei el dato2 [" << mensaje << "] (" << bytesLeidos << " bytes) del pipe" << std::endl;



		std::cout << "Lector: fin del proceso" << std::endl;

		canal.cerrar ();
		exit ( 0 );

	} else {

		// escritor
		std::string dato = "Hola mundo pipes!!         dgdfgdfg              42342342344";
		std::string dato2 = "Hola mundo rrrr!!         dgdfgdfg              rer";
		sleep ( 5 );
		canal.escribir ( static_cast<const void*>(dato.c_str()),dato.size() );
		canal.escribir ( static_cast<const void*>(dato2.c_str()),dato.size() );

		std::cout << "Escritor: escribi el dato [" << dato << "] en el pipe" << std::endl;
		std::cout << "Escritor: fin del proceso" << std::endl;

		// espero a que termine el hijo
		wait ( NULL );

		canal.cerrar ();
		exit ( 0 );
	}
}
