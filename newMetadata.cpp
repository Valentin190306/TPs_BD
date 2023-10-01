#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "Validacion.h"
#include "Validacion.c"

/*
TP 1 - LISTADO ESPECIFICO - LISTADO GENÉRICO

ENUNCIADO N° 3

Alumnos:
    - Mateo Javier Ausqui
    - Valentín Joel Romero Monteagudo
    - Nicolas Huici

Desarrolle un programa para el manejo de una lista de clientes (en realidad puede servir para el manejo de cualquier tipo de archivo),
en la cual el usuario pueda guardar y recuperar fácilmente de la misma los datos que desee (a través de la posición física del registro).
En este caso, a diferencia del anterior, es el propio usuario, interactuando con el programa, quien definirá la estructura de la lista de clientes.
El programa tendrá dos opciones:

    1. Definición (por parte del usuario) de la estructura de la lista de clientes:
        a.  El programa podrá solicitarle al usuario la cantidad de ítems (o campos) que tendrá la lista de clientes.

        b.  Para cada ítem (o campo) el programa solicitará el nombre del campo y la cantidad de caracteres como máximo que tendrá dicho ítem.
            Para facilitar el desarrollo, se asumirá que todos los ítems definidos por el usuario serán de tipo carácter.

        c.  De alguna forma, el usuario le indicará al programa cuando ha terminado de definir la estructura de la lista de clientes.

        d.  Una vez definida la estructura de la lista de clientes, el programa utilizará la misma para realizar el ABM (por posición física de registro, no hay clave primaria)
            y el usuario ya no tendrá posibilidades de cambiar la estructura.

    2.  Utilización de la lista de clientes (ABM sobre la estructura de datos previamente definida), igual que en el programa del punto 1,
        pero ahora utilizando la estructura definida por el usuario. Acorde a los ítems definidos por el usuario en el punto anterior,
        el programa presentará una pantalla en donde el usuario podrá hacer ABM de los datos de la lista de clientes (usando la posición),
        utilizando los ítems que previamente ha definido. El programa deberá indicarle al usuario el nombre del ítem y su correspondiente valor para cada registro.
*/

void crearMetadataCampos(struct metadataRegistro*);

typedef struct metadataCampo {
	char nombreCampo[20];
	int longitudCampo;
	char formato[20];
} metadataCampo;

typedef struct metadataRegistro {
	char nombreArchivo[15];
	int cantidadCampos;
	struct metadataCampo campo[15];
	int cantidadRegistros;
} metadataRegistro;

int main () {
	printf("__ newMetadata");
	printf("\n__ TP 1 - LISTADO ESPECIFICO - LISTADO GENERICO\n\n");
	struct metadataRegistro nuevoMetadata;
	memset(nuevoMetadata.nombreArchivo,'\0', 14);
	crearMetadataCampos(&nuevoMetadata);
	nuevoMetadata.cantidadRegistros = 0;
	FILE * fp = fopen("metadata.dat", "wb");
	if (fp != NULL) {
			fwrite(&nuevoMetadata, sizeof(nuevoMetadata), 1, fp);
		    fclose(fp);
	        printf("\n >> Metadata generada para el archivo %s . . . \n", nuevoMetadata.nombreArchivo);
	        printf("\n << Ingrese - SPACE - para terminar el programa >>");
    }
	else 
	   printf("\n%d >! Error, no se pudo crear el archivo de metadata, reinicie el programa . . . ", __LINE__);
	return 0 ;   
}

void crearMetadataCampos (metadataRegistro * newMeta) {
	int cantidad;
	char filtro[100];
	memset((*newMeta).nombreArchivo,'\0',10);
	printf("\n%d < Ingrese el nombre del archivo : ", __LINE__);
	fgets(filtro, 100, stdin);
	filtro[strcspn(filtro, "\n")] = '\0';
//	strcat(filtro,".txt");
	strcpy((*newMeta).nombreArchivo, filtro);
	
	printf("\n%d < Ingrese la cantidad de campos a definir (maximo de 15 campos): ", __LINE__);
	fgets(filtro, 100, stdin);
	cantidad = EntradaEntera(filtro, 1, 1, 10);
	(*newMeta).cantidadCampos = cantidad;
	(*newMeta).cantidadRegistros = 0;
	
	printf("\n");
	for (int i = 0 ; i < cantidad ; i++) {
		memset((*newMeta).campo[i].nombreCampo, 20, '\0');
		printf("%d < Ingrese el nombre del campo nro. %d: ", __LINE__, i);
		fgets((*newMeta).campo[i].nombreCampo, 20, stdin);
		(*newMeta).campo[i].nombreCampo[strcspn((*newMeta).campo[i].nombreCampo, "\n")] = '\0';
		fflush(stdin);
		printf("%d < Ingrese la longitud del campo [2; 30] nro. %d: ", __LINE__, i);
		fgets(filtro, 100, stdin);
		(*newMeta).campo[i].longitudCampo = EntradaEntera(filtro, 0, 2, 30);
		fflush(stdin);
		memset((*newMeta).campo[i].formato, 20, '\0');
		printf("%d < Ingrese el formato del campo nro. %d: ", __LINE__, i);
		fgets((*newMeta).campo[i].formato, 20, stdin);
		(*newMeta).campo[i].formato[strcspn((*newMeta).campo[i].formato, "\n")] = '\0';
		fflush(stdin);
		printf("\n");
	}
	
	printf("\n ___ Metadata: \n");
		printf("\n _ Nombre del archivo: %s	", (*newMeta).nombreArchivo);
		printf("\n _ Cantidad de campos: %d \n", (*newMeta).cantidadCampos);
		printf("\n Lista de atributos: \n");
	
	for (int i = 0 ; i < (*newMeta).cantidadCampos ; i++)
		printf("\n	%s - %i bytes - %s", (*newMeta).campo[i].nombreCampo, (*newMeta).campo[i].longitudCampo, (*newMeta).campo[i].formato);
	
	printf("\n\n < Ingrese - ENTER - para continuar . . . ");
	getchar();
}