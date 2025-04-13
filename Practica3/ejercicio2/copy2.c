#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  //biblioteca para poder usar los FLAGS
#include <unistd.h>
#include <sys/stat.h> //bilbioteca lstat
#include <unistd.h> //biblioteca para symlink


/*
En este ejercicio lo que tenemos que hacer es modificar lo anterior que teniamos en copy para que ahora
detecte si el fichero que vamos a copiar es un fichero regular o un enlace simbolico.
En el primer caso el programa debera actuar como lo teniamos antes, en el otro caso, debemos crear un enlace
simbolico en vez de copiar el fichero.
Para llevar a cabo esto, he pensado en usar la funcion lstat para obtener la infomracion del fichero,
luego un switch para distinguir casos.

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

	int fdd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644); //abrimo el fichero destino con los FLAGS de solo escrbir y el de truncar, para vaciar el fichero destino en caso de que exista
	if (fdd == -1) { 
                perror("Error al abrir el archivo destino");
                return 1;
        } 
	
//MODIFICIACION EJERCICIO 2

	struct stat sb; //variable que utilizaremos como segundo argumento de la funcion lstat para poder acceder a la informacion del fichero 
	if(lstat (argv[1], &sb) == -1) { //si la llamada a la funcion lstat falla, error
		perror ("lstat");
		exit (EXIT_FAILURE);
	}
	
	switch (sb.st_mode & S_IFMT) { //lo del S_IFMT lo usamos para que solo nos de los bits que hacen referencia al tipo ya que st_mode tb contiene los permisos
		case S_IFREG: //fichero regular

		copy (fdo, fdd); //en caso de que sea un fichero regular, copiamos como haciamos antes
		break;
		
		case S_IFLNK: //fichero de tipo enlace simbolico

		char *buf; //lo utilizaremos para almacenar el puntero que nos devolvera el malloc
		ssize_t buf_size, nbytes; //buf_size para pasarselo al readlink y que sepa el tamaño del enlace, nbytes para almacenar el return del readlink

		buf_size = sb.st_size + 1; //obtenemos el tamaño de la ruta al ficheo que apunta el enlace y le sumamos uno por el caracter nulo

		buf = malloc (buf_size); //reservamos memoria del tamaño del enlace y nos devuelve la ruta del fichero destino
		if(buf == NULL) { //si el mallo falla, error
			perror ("malloc");
			exit (EXIT_FAILURE);
		}

		nbytes = readlink (argv[1], buf, buf_size); //leemos el contenido del fichero origen y almacenamos en buf la ruta del fichero al que apunta el enlace del fichero origen

		if(nbytes == -1) { //si el readlink ha fallado, error
			perror ("readlink");
			free (buf);
			exit (EXIT_FAILURE);
		}
		buf[nbytes] = '\0'; //añadimos el caracter nulo al fichero destino ya que el readlink no es capaz de almacenarlo

		if (unlink(argv[2]) == -1) {  // Si el fichero existe debemos unlinkearlo
			perror("unlink");
			free(buf);
			exit(EXIT_FAILURE);
		}
		

		if(symlink (buf, argv[2]) == -1) { //llamada a symlink con la ruta al archivo que debe apuntar y con la ruta al fichero destino 
			perror ("symlink");
			exit (EXIT_FAILURE);
		}
		free (buf); //liberamos la memoria reservada por el malloc
		break;

		default:
		perror ("error");
	}

	close (fdd); //cerramos el fichero origen
	close (fdo); //cerramos el fichero destino
	return 0; // devolvemos 0 indicando que no ha habido ningun fallo
}
