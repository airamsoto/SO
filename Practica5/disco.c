#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define CAPACITY 5
#define VIPSTR(vip) ((vip) ? "  vip  " : "not vip")

pthread_mutex_t mutex;
pthread_cond_t cond_vip;
pthread_cond_t cond_normal;
int num_clients_inside;
int num_vips_cola, num_normales_cola;

struct arguentos_cliente {
	int id;
	int isVip;
};

void enter_normal_client(int id)
{
	pthread_mutex_lock(&mutex);
	//mientras la discoteca este llena, espero 
	while (num_clients_inside >= CAPACITY) { //falta la condicicio de que no sea vip y que hayan 0 vips

	}
	num_clients_inside++;
	num_normales_cola--;
	pthread_mutex_unlock (&mutex);
}

void enter_vip_client(int id)
{
	pthread_mutex_lock(&mutex); //bloqueamos el mutex
	//mientras la discoteca este llena, espero 
	while (num_clients_inside >= CAPACITY) {//falta alguna condicion
		//hacemos un wait
	}
	num_clients_inside++; //aumentamos el numero de clientes dentro
	num_vips_cola--; //decrementamos el numero de vips en la cola
	pthread_mutex_unlock (&mutex); //desbloqueamos el mutex

}

void dance(int id, int isvip)
{
	printf("Client %2d (%s) dancing in disco\n", id, VIPSTR(isvip));
	sleep((rand() % 3) + 1);
}

void disco_exit(int id, int isvip)
{
	pthread_mutex_lock (&mutex);
	num_clients_inside--;
	/*
	Si el numero de vips es mayor que cero hace el signal a los vips, si no si
	el numero de normal es mayor que cero se lo hace a los normal
	*/
	
	if(num_vips_cola > 0) {
		signal (cond_vip);
	} else if(num_normales_cola > 0) {
		signal (cond_normal);
	}
	pthread_mutex_unlock(&mutex);
}

void *client(void *arg)
{
	struct arguentos_cliente *c = (struct arguentos_cliente*) arg;

	if(c->isVip) { //si el cliente es vip, llamamos a enter_vip_client
		enter_vip_client(c->id); //metemos el cliente vip
	} else { //llamamos a cliente normal
		enter_normal_client (c->id); //metemos el cliente normal
	}	
	disco_exit (c->id, c->isVip);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	FILE *f = fopen (argv[1], 'r'); //revisar el parametro de lectura o escritura
	if (f == NULL) { //si el fichero es NULL significara que ha fallado el fopen
		perror ("fopen"); //indicamos que el error ha sido por el fopen
		exit (EXIT_FAILURE); //hacemos un exit para cortar
	}
		int n;
		fscanf(f, "%d", &n); 
		pthread_attr_t attr;
		pthread_t phs[n];
		
		//inicializar (falta mirar si tengo ya las dos salas de espera)
		pthread_mutex_init(&mutex,  NULL);
		pthread_cond_init(&num_clients_inside,NULL);

		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

		for (int i = 0; i < n; i++){

			struct arguentos_cliente* ptrp=malloc(sizeof(struct arguentos_cliente));
			
			fscanf(f, "%d", &ptrp->isVip);
			ptrp->id= i;
			pthread_create(&phs[i], &attr, client, ptrp);
		}

		for (int i = 0; i < n; i++){
			pthread_join(phs[i], NULL);
		}
		
		//destruir
		pthread_mutex_destroy(&mutex);
 		pthread_cond_destroy(&num_clients_inside);
	
	return 0;
}
