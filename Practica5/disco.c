

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h> //librearias para los hilos

/*
El objetivo de este programa es implementar una discoteca con clientes (hilos) 
que entran, bailan y salen, respetando las reglas de concurrencia.

Tenemos dos tipos de clientes (vips y normales).


REGLAS

Si el aforo esta completo los nuevos clientes deberan esperar a que salga algun cliente
para poder entrar

Si hay clientes vips esperando y normales, se les dara prioridad a los vips

Los clientes iran entrando segun su orden de llegada y su grupo (vip o normal).


En la creacion de cada hilo se le paran dos argumentos:
id: que corresponde con el orden creacion del hilo
isvip: valor que indique si es vip o no
*/

#define CAPACITY 5
#define VIPSTR(vip) ((vip) ? "  vip  " : "not vip")

typedef struct {
	int id; //para representar el id de la creacion del hilo
	int isvip; //0 para clientes normales y 1 para clientes vips
}tCliente; //estructura para representar el hilo (cliente)


int ocupacion = 0; //debemos llevar un registro de los clientes dentro de la discoteca, se incrementara cuando entren y decrementara cuando salga
int esperando_vip = 0; //numero de personas vips esperando, se incremente justo antes de que el vip este con el wait
int esperando_normal = 0; //numero de personas noramles esperando, se incrementa justo antes de que el normal este con el wait

/*
sirve para proteger las variables compartidas para que dos hilos no modifiquen la misma variable (ocupacion, esperando...)
antes de leerla hacemos un lock (para que nadie pueda entrar) y despues un unlock (para que otro pueda entrar)
*/
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

/*
es una variable de condicion y sirve para que un hilo espere de forma segura a que se cumpla una condicion (que haya hueco)
se usa con pthread_cond_wait (&cond, &mutex) donde libera el mutex mientras espera (para que los demas puedan hacer cosas)
y lo bloquea cuando despierta (para que el siga operando)
cuando un cliente sale, hacemos pthread_cond_broadcast(&cond) para despertar a todos los que estaban esperando, y así puedan comprobar si ahora pueden entrar.
*/
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; 

void enter_normal_client(int id)
{
	//a priori deberia ser igual que el vip pero en la condicion añadiendo que no pueden haber vips esperando
	pthread_mutex_lock(&mutex); //bloqueamos el mutex ya que vamos a hacer modificaciones

	while (ocupacion >= CAPACITY || esperando_vip > 0) { //en este caso tenemos las condiciones de que haya hueco y de que no hayan vips esperando
		pthread_cond_wait (&cond, &mutex);//dormimos al  hilo hasta que se cumplan las dos condiciones
	}
	ocupacion++; //un cliente mas en la discoteca
	pthread_mutex_unlock(&mutex); //desbloqueamos el mutex para que puedan usarlo los demas hilos
}

void enter_vip_client(int id)
{
	/*
	primero debemos pensar en el lock y en el unlock del mutex, luego tenemos que ver si la capacidad es mayor que la ocupacion, si no lo es, 
	debemos esperar a que lo sea
	*/

	pthread_mutex_lock(&mutex); //bloqueamos el mutex debido a que vamos a modificar variables
	
	esperando_vip++; //ponemos el cliente vip a la cola

	while (ocupacion >= CAPACITY) { //si la ocupacion es mayor o igaul a la capacidad es decir no hay mas aforo debemos esperar
		pthread_cond_wait(&cond, &mutex); //esperamos dejando el hilo dormir
	}
	ocupacion++; //un cliente mas en la discoteca
	esperando_vip--; //decrementamos el numero de vips esperando debido a que
	pthread_mutex_unlock(&mutex); //desbloqueamos el mutex para que otro hilo pueda usarlo

}

//simula el tiempo que esta en la discoteca el cliente
void dance(int id, int isvip)
{
	printf("Client %2d (%s) dancing in disco\n", id, VIPSTR(isvip));
	sleep((rand() % 3) + 1);
}

//es la funcion que hace el hilo al salir de la discoteca el cliente
void disco_exit(int id, int isvip)
{
	pthread_mutex_lock(&mutex); //bloqueamos el mutex porque vamos a hacer modificaciones 

	ocupacion--; //como ha salido de la discoteca decrementamos el numero de clientes dentro de la discoteca

	pthread_cond_broadcast(&cond); //desbloqueamos el mutex para que otro pueda cogerlo

	pthread_mutex_unlock(&mutex); //hacemos un broadcast para despertar a todos y que al que le toque que entre

}
//funcion de entrada de los clientes
void *client(void *arg)
{
	tCliente *cliente = (tCliente*) arg; //debemos hacer un casteo para poder operar con  los datos de los cliente
	
	if(cliente->isvip) { //si es vip, llamamos al enter_vip 
		enter_vip_client(cliente->id);
	} else  { //si no, es un cliente normal
		enter_normal_client(cliente->id);
	}

	dance(cliente->id, cliente->isvip); //una vez esta dentro, se pone a bailar
	disco_exit (cliente->id, cliente->isvip); //una vez ha bailado debe salir de la discoteca

	//ver si el free del hilo que creamos hay que hacerlo aqui o donde hacemos el malloc


}

int main(int argc, char *argv[])
{
	/*
	Leer los datos del archivo:

    El número total de clientes.

    La lista de si cada uno es VIP (1) o no (0).

Reservar memoria para cada cliente (crear un array de estructuras o mallocs individuales).

Crear un hilo por cliente:

    Cada hilo debe ejecutar la función client(...).

    A cada hilo se le pasa su id y si es vip.

Esperar a que todos los hilos terminen:

    Usar pthread_join para cada hilo.

Liberar recursos:

    Destruir los mutex y cond.

    Liberar memoria si hiciste malloc.
	*/
	
	return 0;
}