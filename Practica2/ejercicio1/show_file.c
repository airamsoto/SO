#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char* argv[]) { //argc es el numero de argumentos que le pasamos
				  //en este caso es dos ya que el primer arg es el mismo y el otro es el fichero que vamos a leer (check_showfile)
	FILE* file=NULL;
	int c,ret;

	if (argc!=2) {
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		exit(1);
	}

	/* Open file */
	if ((file = fopen(argv[1], "r")) == NULL) //el fopen devuelve un FILE* en caso de abrirse  bien
		err(2,"The input file %s could not be opened",argv[1]); //si no se ha abierto bien (devuelve NULL)

	/* Read file byte by byte */
/*
	while ((c = getc(file)) != EOF) {
		// Print byte to stdout
		ret=putc((unsigned char) c, stdout);
		if (ret==EOF){
			fclose(file);
			err(3,"putc() failed!!");
		}
	}*/
//Ahora debemos sustituir el metodo getc por fread y putc por fwrite
	while (fread (&c, sizeof(char), 1, file) == 1)  {
/*
El fread lo que devuelve es un 1 (si ha leido algo) o un 0 (si no ha leido nada)
por lo que leeremos mientras haya leido algo.
Los argumentos son:
&c (direccion donde vamos a almacenar lo que vayamos leyendo
sizeof(char) (tamaño de los datos que estamos leyendo)
1 (numero de elementos a leer, en este caso vamos char a char)
file (puntero al archivo abierto del que estamos leyendo)
*/
		fwrite (&c, sizeof(char),1, stdout);
/*
El fwrite devuelve el numero de elementos escritos, pero en este caso no nos interesa saberlo
Los argumentos son:
&c (la direccion de lo que vamos a escribir)
sizeof(char) (tamaño de lo que vamos a escribir)
1 (numeero de elementos que vamos a escribir)
stdout (donde queramos que se escriba, en este caso es por consola)
*/
	}

	fclose(file); //siempre debemos cerrar los fichero que abramos
	return 0;
}
