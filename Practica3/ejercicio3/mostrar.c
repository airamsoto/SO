#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

/*
Debemos simular la funcionalidad de cat con unos matices. Utilizaremos dos opciones -n N  y -e.

    -n N: indica que queremos saltarnos N bytes desde el comienzo del fichero o mostrar únicamente los N últimos bytes del fichero. Que se haga una cosa o la otra depende de la presencia o no de un segundo flag -e. Si el flag -n no aparece N tomará el valor 0.
    -e: si aparece, se leerán los últimos N bytes del fichero. Si no aparece, se saltarán los primeros N bytes del fichero.
*/


int main(int argc, char *argv[])
{
	int opt, e = 0, N;
	while ((opt = getopt(argc, argv, "n:e")) != -1) {
		switch (opt) {
			case 'n': 
			//sacar el numero N
			break;
			case 'e':
			e = 1;
			break;
		}
	}
	int fd = fopen (argv[1], 'r');
	if(fd == NULL) {
		perror ("fopen");
		exit (EXIT_FAILURE);
	}
	int jose = 0;

	if (e) {
		//si hay opcion e, entonces leeremos los ultimos N bytes del fichero. Para ello pondremos el apuntador al final del fihchero y usaremos N como negativo
		jose = lseek (fd, -N, SEEK_END);
	} else {
		//si no hay opcion e, entonces nos saltaremos los primeros N bytes
		jose = lseek (fd, N, SEEK_SET);
	}	

	if (jose == -1) {
		perror ("lseek");
		exit (EXIT_FAILURE);
	}

	//ahora tenemos que leeser desde el punto en el que estamos hasta el final del fichero
	
	return 0;
}
