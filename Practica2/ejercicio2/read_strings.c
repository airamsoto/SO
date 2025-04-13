#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>


/** Loads a string from a file.
*
* file: pointer to the FILE descriptor
*
* The loadstr() function must allocate memory from the heap to store
* the contents of the string read from the FILE.
* Once the string has been properly built in memory, the function returns
* the starting address of the string (pointer returned by malloc())
*
* Returns: !=NULL if success, NULL if error
*/
char *loadstr(FILE *file)
{
   char ch;        //Variable para guardar el caracter que leemos
   int length =0;
   long start_pos = ftell(file); //Guarda la posición inicial del archivo
  
   while(fread(&ch, sizeof(char),1,file) == 1 && ch!= '\0'){
       length++;       //guardamos dónde acaba la palabra leída.
   }
   fseek(file, start_pos, SEEK_SET);
  
   char* str= (char*)malloc((length +1) * sizeof(char));
  
   // Leer la cadena completa del archivo.  IMPORTANTE: length + 1
   if (fread(str, sizeof(char), length + 1, file) != (length + 1)) {
       free(str); // Libera la memoria si ocurre un error al leer.
       return NULL;
   }


   return str;
}


int main(int argc, char *argv[])
{
   /* To be completed */
   FILE* fichero;
   char* str;


   fichero = fopen (argv[1], "r");
   if (fichero == NULL) {
       perror("Error al abrir el archivo");
       exit(EXIT_FAILURE);
   }
   while((str=loadstr(fichero))!= NULL){
       printf("%s\n", str);
       free(str);
       }
   fclose(fichero);
   return 0;
}



