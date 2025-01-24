#variables definidas
CC = gcc
CFLAGS = -Wall -g
LIBS = -lm

#objetivo(target): creacion de archivo.o a partir del archivo.c 
%.o: %.c
	#regla de contruccion
	$(CC) $(CFLAGS) -c -o $@ $<

#objetivo(target): creacion del ejecutable a partir de aux.o init.o aux.h
all : aux.o init.o aux.h
	#regla de construccion
	gcc $(CFLAGS) -o init aux.o init.o $(LIBS) #libreria LIBS es matematica, necesaria

#objetivo(especial): para que la herramienta make, ante el paso del argumento clean, no lo interprete como un fichero
.PHONY: clean

#objetivo(target): eliminacion del archivo.o
clean:
	#regla de construccion
	-rm *.o init
