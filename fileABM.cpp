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


int leerMetadata(struct metadataRegistro*);
int actualizarMetadata (struct metadataRegistro*);

int altaDeRegistro(struct metadataRegistro*);
int bajaDeRegistro(struct metadataRegistro*);
int modificarRegistro(struct metadataRegistro*);
int mostrarRegistros(struct metadataRegistro*);

typedef struct metadataCampo {
	char nombreCampo[20];
	int longitudCampo;
	char formato[20];
} metadataCampo;

typedef struct metadataRegistro {
	char nombreArchivo[15];
	int cantidadCampos;
	struct metadataCampo campo[15];
} metadataRegistro;

int menuABM (void) {
	char filtro[100];
	int seleccion = 0;
	printf("\n __ Modificar archivo: ");
	printf("\n 1 : Alta / Agregar registro");
	printf("\n 2 : Baja / Eliminar registro");
	printf("\n 3 : Modificar registro");
	printf("\n 4 : Mostrar Archivo de Datos");
	printf("\n 0 : Terminar programa");
    printf("\n%d < Ingrese la accion: ", __LINE__);	
	fgets(filtro, 100, stdin);
	seleccion = EntradaEntera(filtro, 0, 0, 4);
	return 	seleccion ;
}

int main (void) {
	int opcion ;
	struct metadataRegistro metadata;
	if (leerMetadata(&metadata) == -1)
       return -1; 	
	while((opcion = menuABM())!=0)
	{
	  //  printf("\n _ Cantidad de registros: %d \n", metadata.cantidadRegistros);	
		switch (opcion)
		 {
			case 1 : {
				//alta de datos
				altaDeRegistro(&metadata);
				break;
			} 
			case 2 : {
				//baja de datos
				bajaDeRegistro(&metadata);
				break;
			} 
			case 3 : {
				//modifica datos
				modificarRegistro(&metadata);
				break;				
			} 
			case 4 : {
				//mostrar datos
				mostrarRegistros(&metadata);
				break;				
			}
		}
	} 
	printf("\n%d // Terminando programa . . . ", __LINE__);
	actualizarMetadata(&metadata);
	return 0 ;
}


int altaDeRegistro (metadataRegistro * registro) {
	char continuar;
	FILE * fp = fopen((*registro).nombreArchivo, "a");
	if (fp != NULL) {
		do {
			printf("\n > Se requieren ingresar %d campos . . .", (*registro).cantidadCampos);
			for (int i = 0 ; i < (*registro).cantidadCampos ; i++) {
				char buffer[(*registro).campo[i].longitudCampo+1];
				memset(buffer, (*registro).campo[i].longitudCampo+1, '\0');
				printf("\n\n%d ln > Campo: %s - %s - %d bytes", __LINE__, (*registro).campo[i].nombreCampo, (*registro).campo[i].formato, (*registro).campo[i].longitudCampo);
				printf("\n%d < Ingrese el contenido del campo: ", __LINE__);
				fgets(buffer, ((*registro).campo[i].longitudCampo), stdin);
				fflush(stdin);
				buffer[strcspn(buffer, "\n")] = '\0';
				fputs(buffer, fp);
				fputs(" ", fp);	
			}
			fputs("\n", fp);												
			printf ("\n%d < Continuar agregando registros? (y/n): ", __LINE__);
			scanf("%c", &continuar);
			fflush(stdin);
		} while (continuar != 'n');
		fclose(fp);
		return 0 ;
	} 
	printf("\n%d >! Error, no se pudo abrir o no existe el archivo %s, reinicie el programa . . .\n", __LINE__, ((*registro).nombreArchivo));
	return -1;
}

int bajaDeRegistro (metadataRegistro * registro) {				
	int posicion, i ;
	int bufferCap = 0;
	
	for (i = 0 ; i < (*registro).cantidadCampos ; i ++) 
		bufferCap += (*registro).campo[i].longitudCampo;
	bufferCap += ((*registro).cantidadCampos);
	char bufferReg[bufferCap+1];
	
	memset(bufferReg, '\0', bufferCap+1);
	FILE * fp = fopen((*registro).nombreArchivo, "r");
	if (fp != NULL) {
		FILE * nfp = fopen("archivoTemporal.txt", "w");
		printf("\n%d < Ingrese la posicion ordinal del registro a eliminar: ", __LINE__);
		scanf("%d",&posicion);
		fflush(stdin);
		
		for (i = 1 ; fgets(bufferReg, bufferCap, fp) ; i++) {
			if (posicion != i) {
		    	fputs(bufferReg, nfp);
			}
		}	
		fclose(fp);
		remove((*registro).nombreArchivo);
		fclose(nfp);
		rename("archivoTemporal.txt", (*registro).nombreArchivo);
		return 0 ;
	}
	else {
	   	printf("\n%d >! Error, no se pudo abrir o no existe el archivo %s, reinicie el programa . . .\n", __LINE__, ((*registro).nombreArchivo));
	    return -1;   	
	}
}

int modificarRegistro (metadataRegistro * registro) {
	int posicion, i, y;
	long bufferCap = 0;
	
	for (i = 0 ; i < (*registro).cantidadCampos ; i ++) 
		bufferCap += (*registro).campo[i].longitudCampo;
	bufferCap += (*registro).cantidadCampos;
	char bufferReg[bufferCap+1];
	memset(bufferReg, '\0', bufferCap+1);
	
	FILE * fp = fopen((*registro).nombreArchivo, "r");
	if (fp != NULL) {
		FILE * nfp = fopen("archivoTemporal.txt", "w");
		printf("\n%d < Ingrese la posicion ordinal del registro a modificar: ", __LINE__ );
		scanf("%d",&posicion);
		fflush(stdin);
		for (i = 1 ; fgets(bufferReg, bufferCap, fp); i++) {
			if (posicion != i) {
				fputs(bufferReg, nfp);
			}
			else {  // encontre el registro que quiero modificar
				printf(" Registro leido %s \n ",bufferReg);			
				printf("\n > Se requieren re-ingresar %d campos . . .", (*registro).cantidadCampos);			
				for (int i = 0 ; i < (*registro).cantidadCampos ; i++) {
					char buffer[(*registro).campo[i].longitudCampo+1];
					memset(buffer, '\0', (*registro).campo[i].longitudCampo+1);
					printf("\n\n%d f > Campo: %s - %s - %d bytes", __LINE__, (*registro).campo[i].nombreCampo, (*registro).campo[i].formato, (*registro).campo[i].longitudCampo);
					printf("\n%d < Ingrese el contenido del campo: ", __LINE__);
					fgets(buffer, (*registro).campo[i].longitudCampo, stdin);
					fflush(stdin);
					fputs(buffer, nfp);
					fputs(" ", nfp);
				}
				fputs("\n", nfp);
			}		
		}
		fclose(fp);
		remove((*registro).nombreArchivo);
		fclose(nfp);
		rename("archivoTemporal.txt", ((*registro).nombreArchivo));
		return 0;
	}
	printf("\n%d >! Error, no se pudo abrir o no existe el archivo %s, reinicie el programa . . .\n", __LINE__, ((*registro).nombreArchivo));
	return -1;
}

int mostrarRegistros (metadataRegistro * registro) {
	char reg[100];
	int i = 1;
	memset(reg,'\0',100);
	FILE * fp = fopen((*registro).nombreArchivo, "r");
	if (fp != NULL) {
		printf("\n _ Archivo: %s\n", (*registro).nombreArchivo);
	    while(fgets(reg, 100, fp)) {
	      	printf("	reg[%d] - %s", i, reg);
	      	i++;
		}
	    fclose(fp);
	    return 0;
	}
	printf("\n%d >! Error, no se pudo abrir o no existe el archivo %s, reinicie el programa . . .\n", __LINE__, ((*registro).nombreArchivo));
	return -1;
}

int leerMetadata(metadataRegistro * metadata){	
	FILE * fpm = fopen("metadata.dat", "rb");
	if (fpm == NULL) {
		printf("\n%d >! Error, no se pudo leer el archivo de metadata, reinicie el programa . . . ", __LINE__);
		return -1;
	} 	
	else {
	  	fread(metadata, sizeof(struct metadataRegistro), 1, fpm);
	  	fclose(fpm);
	  	printf("\n Archivo: %s\n", (*metadata).nombreArchivo);
		printf("\n ___ Metadata: \n");
		printf("\n _ Nombre del archivo: %s	", (*metadata).nombreArchivo);
	  	printf("\n _ Cantidad de campos: %d \n", (*metadata).cantidadCampos);		
	  	printf("\n _ Lista de atributos: ");
	  	for (int i = 0 ; i < (*metadata).cantidadCampos ; i++)
			printf("\n	%s - %i bytes - %s", (*metadata).campo[i].nombreCampo, (*metadata).campo[i].longitudCampo, (*metadata).campo[i].formato);
	  	printf("\n");		
    }
    return 0;
}

int actualizarMetadata (struct metadataRegistro * metadata) {
	FILE * nfp = fopen("metadata.dat", "wb");
	 if (nfp == NULL) {
        perror("Error al abrir metadata.dat");
        return -1;
    }
    size_t elementosEscritos = fwrite(metadata, sizeof(struct metadataRegistro), 1, nfp);
    fclose(nfp);
	if (elementosEscritos != 1) {
        perror("Error al escribir en metadataActualizada.dat");
        return -1;
    }
    return 0;
}
