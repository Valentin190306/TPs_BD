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

bool altaDeRegistro(struct metadataRegistro);
void bajaDeRegistro(struct metadataRegistro);
void mostrarRegistro(char[], struct metadataRegistro);
bool modificarRegistro(struct metadataRegistro);

typedef struct metadataCampo {
	char nombreCampo[10];
	int longitudCampo;
	char formato[10];
} metadataCampo;

typedef struct metadataRegistro {
	char nombreArchivo[15];
	int cantidadCampos;
	struct metadataCampo campo[10];
	unsigned long cantidadRegistros;
} metadataRegistro;

void menuABM () {
	printf(" __ Modificar archivo: ");
	printf(" 1 : Alta / Agregar registro");
	printf(" 2 : Baja / Eliminar registro");
	printf(" 3 : Modificar registro");
	printf(" 0 : Terminar programa");
}

int main () {
	bool continuar = true;
	int seleccion = 0;
	char filtro[100];
	
	do {
		menuABM ();	
		printf("\n%d < Ingrese la accion: ", __LINE__);	
		fgets(filtro, 100, stdin);
		seleccion = EntradaEntera(filtro, 0, 0, 3);
		
		FILE * fpm = fopen("metadata.bin", "rb+");
		struct metadataRegistro metadata;
		
		if (fpm == NULL) {
			printf("\n%d >! Error, no se pudo leer el archivo de metadata, reinicie el programa . . . ", __LINE__);
			seleccion = 0;
		} 
		
		else fread(&metadata, sizeof(metadataRegistro), 1, fpm);
		
		switch (seleccion) {
			case 0 : {
				printf("\n%d// Terminando programa . . . ", __LINE__);
				continuar = false;	
			} break;
			
			case 1 : {
				continuar = altaDeRegistro(metadata);
			} break;
			
			case 2 : {
				bajaDeRegistro(metadata);
			} break;
			
			case 3 : {
				continuar = modificarRegistro(metadata);				
			} break;
		}
	} while (continuar);
}

bool altaDeRegistro (metadataRegistro registro) {
	char continuar;
	
	int cantidadCampos = (int) (sizeof(registro.campo) / sizeof(metadataCampo));
	
	FILE * fp = fopen(registro.nombreArchivo, "a+");
	
	if (fp != NULL) {
		do {
			printf("\n > Se requieren ingresar %d campos . . .", cantidadCampos);
			
			for (int i = 0 ; i < cantidadCampos ; i++) {
				char buffer[registro.campo[i].longitudCampo+1];
				memset(buffer, registro.campo[i].longitudCampo+1, '\0');
				printf("\n\n%d f > Campo: %s	-	%s	-	%d bytes", __LINE__, registro.campo[i].nombreCampo, registro.campo[i].formato, registro.campo[i].longitudCampo);
				printf("\n%d < Ingrese el contenido del campo: ", __LINE__);
				fgets(buffer, registro.campo[i].longitudCampo, stdin);
				fputs(buffer, fp);
			}
			
			fputs("\n", fp);
			registro.cantidadRegistros++;							
									
			printf ("\n%d < Continuar agregando registros? (y/n): ", __LINE__);
			scanf("%c", &continuar);
		} while (continuar != 'n');
		fclose(fp);
		return true;
	} 
	
	else {
		printf("\n%d >! Error, no se pudo abrir/crear el archivo %s, reinicie el programa . . .", __LINE__, registro.nombreArchivo);
		return false;
	}	
}

void bajaDeRegistro (metadataRegistro registro) {
	int cantidadCampos = (int) (sizeof(registro.campo) / sizeof(metadataCampo));
	int posicion, i, y;
	char verificacion;
	
	long bufferCap = 0;
	
	for (i = 0 ; i < cantidadCampos ; i ++) 
		bufferCap += registro.campo[i].longitudCampo;

	char bufferReg[bufferCap+1];
	memset(bufferReg, bufferCap+1, '\0');
	
	FILE * fp = fopen(registro.nombreArchivo, "r");
	FILE * nfp = fopen("archivoTemporal.txt", "w");
	
	if (fp != NULL && nfp != NULL) {
		if (registro.cantidadRegistros != 0) {
			printf("\n%d < Ingrese la posicion del registro a eliminar [0 ; %d]", registro.cantidadRegistros);
			scanf("%d", &posicion);
			fflush(stdin);
			
			for (i = 0 ; i < registro.cantidadRegistros ; i++) {
				char buffer[registro.campo[i].longitudCampo+1];
				memset(buffer, registro.campo[i].longitudCampo+1, '\0');
				
				if (posicion != i-1) {
					for (y = 0 ; y < cantidadCampos ; y++) {
						fgets(buffer, registro.campo[y].longitudCampo, fp);
						fputs(buffer, nfp);
					}
					fgets(buffer, registro.campo[y].longitudCampo, fp);
					fputs(buffer, nfp);
				}
				
				else {
					fgets(bufferReg, bufferCap, fp);
					mostrarRegistro(bufferReg, registro);
					printf("\n%d < Desea eliminar este registro? (y/n): ");
					scanf("%c", &verificacion);
					
					if (verificacion == 'n')
						break;							
				}
				registro.cantidadRegistros--;			
			}
			
			if (verificacion == 'n')
				remove("archivoTemporal.txt");
			else
				remove(registro.nombreArchivo);
				rename("archivoTemporal.txt", registro.nombreArchivo);
		}
		
		else printf ("\n%d > Archivo vacio . . .");
	}
	
	else printf("\n%d >! Error, no se pudo abrir o no existe el archivo %s, reinicie el programa . . .", __LINE__, registro.nombreArchivo);
}

void mostrarRegistro (char bufferReg, metadataRegistro registro) {
	int cantidadCampos = (int) (sizeof(registro.campo) / sizeof(metadataCampo));
	int tamanoParcial, posicion = 0;
	
	for (int i = 0 ; i < cantidadCampos ; i++) {
		// Calcular el tamaño del segmento actual
        tamanoActual = (strlen(bufferReg) - posicion < registro.campo[i].longitudCampo) ? (gitudCadena - posicion) : tamanoSegmento;

        // Copiar el segmento actual a una nueva cadena
        char segmento[tamanoActual + 1]; // +1 para el carácter nulo
        strncpy(segmento, cadena + posicion, tamanoActual);
        segmento[tamanoActual] = '\0'; // Agregar el carácter nulo al final

        // Imprimir o hacer lo que necesites con el segmento
        printf("Segmento: %s\n", segmento);

        // Mover la posición al siguiente segmento
        posicion += tamanoSegmento;
	}
}

bool modificarRegistro (metadataRegistro registro) {
	
	return true;
}