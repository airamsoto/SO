#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  //biblioteca para poder usar los FLAGS
#include <unistd.h>
/*
Deberá  realizar la copia en bloques de 512B usando array local (los va leyendo y los va copiando)
Si lo que queda por leer no es multiplo de 512, se leera lo que queda (consultar RETURN VALUE en read)
Se escribirán tantos bytes como se hayan leido

*/
void copy(int fdo, int fdd) //fdo:fichero origen (a copiar) fdd: fichero destino
{
	char array[512]; //array donde vamos a ir copiando lo que vayamos leyendo
	ssize_t bytes_leidos; //variable para saber cuantos bytes hemos leido (lo que nos ha devuelto el read)
	while ((bytes_leidos= read (fdo, array, sizeof(array))) > 0) { //cuando el read nos devuelva un numero menor de 0, significara que ya no hay nada mas que leer

        ssize_t bytes_escritos =  write (fdd, array, bytes_leidos);//utilizamos un write para copiar lo que hemos almacenado del read en array, en el fichero destino fdd, con la cantidad de bytes "bytes_leidos"
	}

	close(fdo); //cerramos el fichero origen
	close(fdd); //cerramos el fichero destino
}

int main(int argc, char *argv[])
{
	int fdo = open(argv[1],O_RDONLY); //uso open porque lo que le necesito pasar al copy es el fd, el primer argumento "argv[1]" es el nombre que le pasamos del fichero origen
	if (fdo == -1) { //si el open devuelve -1, significa que ha habido un error al abrir el fihcero origen
		perror("Error al abrir el archivo origen"); //usamos perrror para lanzar un error
		return 1; //return 1 para indicar que ha habido algun tipo de error
	}

	int fdd = open(argv[2], O_WRONLY | O_TRUNC); //abrimo el fichero destino con los FLAGS de solo escrbir y el de truncar, para vaciar el fichero destino en caso de que exista
	if (fdd == -1) { 
                perror("Error al abrir el archivo destino");
                return 1;
        } 

	copy (fdo, fdd); //llamada al metodo copy con los descriptores de fichero origen y destino
	close (fdd); //cerramos el fichero origen
	close (fdo); //cerramos el fichero destino
	return 0; // devolvemos 0 indicando que no ha habido ningun fallo
}
