#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>


int main(int argc, char* argv[])
{
   FILE* fichero;
   int i;
   fichero = fopen (argv[1], "w");
   if (fichero == NULL) {
       perror("Error al abrir el archivo");
       exit(EXIT_FAILURE);
   }

   for (int i = 2; i < argc; i++) {
       fwrite (argv[i], sizeof(char), strlen (argv[i]), fichero);
       fwrite("\0", sizeof(char), 1, fichero);      // Escribir el carácter nulo
   }

   fclose(fichero);
   return EXIT_SUCCESS;
}


