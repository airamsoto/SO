#include <stdlib.h>
#include <stdio.h>
#include <pthread.h> //biblioteca para hilos
/*
Este programa va a crear hilos con una funcionalidad especificada
a traves de los argumentos que se le pasen a la hora 
de crearlos.
Recibiran un puntero que apunte a una estructura con dos campos
struct x {
	int numeroHilo;
	char c; -> indicara si es P (prioritario) o N (no prioriotario)
}
Debemos crear una variable para el argumento de forma dinamica,
inicializar la variable (numero de hilo y prioridad) donde los pares
seran prioritarios y los impares NO prioritarios
Tendremos que liberar memoria e imprimir sus argumentos

CREAR HILOS con pthread_create
int pthread_create(pthread_t *restrict thread,
                          const pthread_attr_t *restrict attr,
                          void *(*start_routine)(void *),
                          void *restrict arg);


*/
struct thread_info {
	int numThread;
	char c;
};

void *thread_usuario(void *arg)
{
	struct thread_info *info = arg; //creamos la variable para almacenar la info del hilo
	//ahroa tenemos que ir imprimiendo la informacion
	printf("Hilo %d: Tipo: %c\n", info->numThread, info->c);
    
    // Liberar la memoria utilizada para este hilo
    free(info);
    
    return NULL;

}
int main(int argc, char* argv[])
{
	//comprobamos si se ha pasado el numero de hilos a crear como argumento
	if (argc < 2) {
       perror("Especifica el numero de hilos"); //error
	   exit (EXIT_FAILURE);
    }

    int num_hilos = atoi(argv[1]); //sacamos el numero de hilos a crear a partir del primer agumento y lo pasamos a int con atoi
	//pthread_t threads =  //creamos un array para almacenar todos los hilos

	for (int i = 0; i < num_hilos; i++)
	{
		struct thread_info *info = malloc (sizeof(info)); //reservamos memoria con el uso de mallo de tamaño la estrcutura de info
		if(info == NULL) { //si falla el malloc error
			perror("malloc");
			exit (EXIT_FAILURE);
		}
	}
	

    return EXIT_SUCCESS;
}