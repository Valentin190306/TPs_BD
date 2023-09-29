/* GLORIA UT DEUS
 * conatus me
 */

/* Materia 11077 Unlu BD I
 *
 * Alumnos: Valentin Joel Romero Monteagudo 190306
 *          Nicolas Guillermo Huici         165
 *          Mateo Javier Ausqui             190236
 */

#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>

#define MAX_TAM_CADENA 40
#define MAX_COLISIONES_POR_POS 15

const char ruta_archivo[200] = "tabla.dat";

typedef struct Cliente {

    char nombres[MAX_TAM_CADENA];
    char apellido[MAX_TAM_CADENA];
    int cod_cli;

}cliente_t;

typedef struct Registro_tabla {

    cliente_t cliente;
    int tam_colisiones;
    cliente_t colisiones[MAX_COLISIONES_POR_POS];

}registro_tabla_t;

//Según mis cálculos, a pesar de ser ineficiente
//la estructura hash que guarda datos de este programa,
//ocupara casi un megabyte de espacio solamente (949200 bytes)
//Dado que sólo se guardarán 700 registros.
//Se que es negligente en cuanto a la reutilización de este código
//Pero no tengo más tiempo para dedicarle.
//Soluciona lo que se pide.

//La funcion hash es un mod 700, MAX_COLISIONES quedaría en 14.28, redondeado a 15 por las 198 posiciones restantes

int hash (int cod_cli) {

    return (cod_cli%700);
}

bool cliente_nulo (cliente_t cliente) {

    return (cliente.cod_cli == 0);
}

//toma posiciones a partir de 14, no se utiliza con input externo
void mover_colisiones_desde_posicion (registro_tabla_t* reg_tabla, int pos) {

    int i = pos;

    while (!(cliente_nulo(reg_tabla->colisiones[i])) && (i < MAX_COLISIONES_POR_POS - 1)) {

        strcpy(reg_tabla->colisiones[i].nombres, reg_tabla->colisiones[i+1].nombres);
        strcpy(reg_tabla->colisiones[i].apellido, reg_tabla->colisiones[i+1].apellido);
        reg_tabla->colisiones[i].cod_cli = reg_tabla->colisiones[i+1].cod_cli;

        i++;

    }

}

void reemplazar_cliente_con_primer_colision (registro_tabla_t* reg_tabla) {

    strcpy(reg_tabla->cliente.nombres, reg_tabla->colisiones[0].nombres);
    strcpy(reg_tabla->cliente.apellido, reg_tabla->colisiones[0].apellido);
    reg_tabla->cliente.cod_cli = reg_tabla->colisiones[0].cod_cli;

    //pasar el resto de colisiones una posicion para adelante
    mover_colisiones_desde_posicion(reg_tabla, 0);

}

bool codigo_cliente_valido (int cod_cli) {

    return ((cod_cli > 0) && (cod_cli < 9999));
}

//En display del sistema los códigos tendrán cuatro dígitos, a pesar de tener inclusive menos cifras significativas.
//Ej: 1 -> 0001
cliente_t recibir_cliente () {

    cliente_t nuevo_cliente;

    printf("Ingrese los nombres del cliente [1-40 caracteres]: ");

    char aux[MAX_TAM_CADENA];
    scanf("%s", aux);
    strcpy(nuevo_cliente.nombres, aux);
    strcat(nuevo_cliente.nombres, "\0");

    while (getchar() != '\n');

    printf("Ingrese el apellido del cliente [1-40 caracteres]: ");

    scanf("%s", aux);
    strcpy(nuevo_cliente.apellido, aux);
    strcat(nuevo_cliente.apellido, "\0");

    while (getchar() != '\n');

    int cod_buffer = 0;

    printf("Ingrese el código del cliente[1-4 dígitos]: ");
    scanf("%i", &(cod_buffer));

    while (getchar() != '\n');

    while (!codigo_cliente_valido(cod_buffer)) {

        printf("Código no válido.\n");
        printf("Ingrese el código del cliente: ");
        scanf("%i", &(cod_buffer));

        while (getchar() != '\n');

    }

    nuevo_cliente.cod_cli = cod_buffer;

    return nuevo_cliente;
}

void alta (registro_tabla_t* tabla_hash, char nombres[], char apellido[], int cod_cli) {

    int pos = hash(cod_cli);

    if (cliente_nulo(tabla_hash[pos].cliente)) {

        strcpy(tabla_hash[pos].cliente.nombres, nombres);
        strcpy(tabla_hash[pos].cliente.apellido, apellido);
        tabla_hash[pos].cliente.cod_cli = cod_cli;

    } else {

        //Revisa si tiene igual codigo que alguno. si no, coloca en la proxima posicion libre de colisiones.

        registro_tabla_t* reg_tabla = &tabla_hash[pos];

        bool copia = false; //Controla que cod_cli no sea repetido.
        bool colocado = false; //Controla que el cliente sea colocado en colisiones.

        if (reg_tabla->cliente.cod_cli == cod_cli) {

            copia = true;
            printf("\nCódigo de cliente repetido.");

        } else {

            int i = 0;

            while (!(copia) && !(colocado) && (i < MAX_COLISIONES_POR_POS)) {

                if (reg_tabla->colisiones[i].cod_cli == cod_cli) { //Chequea que no sea copia

                    copia = true;
                    printf("\nCódigo de cliente repetido.");

                } else if (cliente_nulo(reg_tabla->colisiones[i])) { //Chequea que no sea vacío y copia si es vacío

                    strcpy(reg_tabla->colisiones[i].nombres, nombres);
                    strcpy(reg_tabla->colisiones[i].apellido, apellido);
                    reg_tabla->colisiones[i].cod_cli = cod_cli;

                    colocado = true;

                    reg_tabla->tam_colisiones += 1;

                }

                i++;

            }

        }

    }

}

void baja (registro_tabla_t* tabla_hash, int cod_buscado) {

    int pos = hash(cod_buscado);

    if (cliente_nulo(tabla_hash[pos].cliente)) { //Ningún cliente asignado a la posición donde debería estar el cliente a dar de baja

        printf("\nCódigo cliente no coincide con clientes dados de alta.");

    } else {

        registro_tabla_t* reg_tabla = &tabla_hash[pos]; //Si esto no funciona, usar directo tabla_hash, en especial para funciones.

        bool borrado = false; //Controla que el cliente sea borrado, en caso de encontrarlo. Si permanece false, no fue encontrado.

        if (reg_tabla->cliente.cod_cli == cod_buscado) {

            //Copia primera posicion de colisiones en cliente
            reemplazar_cliente_con_primer_colision(reg_tabla);

            if (reg_tabla->tam_colisiones > 0) {

                reg_tabla->tam_colisiones -= 1;

            }

            borrado = true;
            printf("\nCódigo de cliente repetido.");

        } else { //Se revisan las colisiones para verificar que el que hay que dar de baja este. Si esta se da de baja.

            int i = 0;

            while (!(cliente_nulo(reg_tabla->colisiones[i])) && (i < MAX_COLISIONES_POR_POS - 1)
                   && !(borrado)) {

                if (reg_tabla->colisiones[i].cod_cli == cod_buscado) {

                    mover_colisiones_desde_posicion(reg_tabla, i);

                    reg_tabla->tam_colisiones -= 1;
                    borrado = true;

                }

                i++;

            }

            //Chequea última posición y borra si está allí

            if (reg_tabla->colisiones[MAX_COLISIONES_POR_POS - 1].cod_cli == cod_buscado) {

                strcpy(reg_tabla->colisiones[MAX_COLISIONES_POR_POS - 1].nombres, " ");
                strcpy(reg_tabla->colisiones[MAX_COLISIONES_POR_POS - 1].apellido, " ");
                reg_tabla->colisiones[MAX_COLISIONES_POR_POS - 1].cod_cli = 0;

                reg_tabla->tam_colisiones -= 1;

                borrado = true;

            }

        }

        if (!borrado) {

            printf("\nEl cliente a borrar no se encuentra entre los clientes guardados.");

        }

    }

}

void modificar (registro_tabla_t* tabla_hash, char nombres[], char apellido[], int cod_buscado) {

    int pos = hash(cod_buscado);

    if (cliente_nulo(tabla_hash[pos].cliente)) { //Ningún cliente asignado a la posición donde debería estar el cliente a modificar

        printf("\nCódigo cliente no coincide con clientes dados de alta.");

    } else {

        registro_tabla_t* reg_tabla = &tabla_hash[pos];

        bool no_hallado = false;
        bool modificado = false;

        if (reg_tabla->cliente.cod_cli == cod_buscado) {

            strcpy(tabla_hash[pos].cliente.nombres, nombres);
            strcpy(tabla_hash[pos].cliente.apellido, apellido);

            modificado = true;

        } else {

            int i = 0;

            while (!(no_hallado) && !(modificado) && (i < MAX_COLISIONES_POR_POS)) {

                if (reg_tabla->colisiones[i].cod_cli == cod_buscado) {

                    strcpy(reg_tabla->colisiones[i].nombres, nombres);
                    strcpy(reg_tabla->colisiones[i].apellido, apellido);

                    modificado = true;

                } else if (cliente_nulo(reg_tabla->colisiones[i])) { //Fin de posibles chequeos

                    no_hallado = true;

                }

                i++;

            }

        }

    }

}

void mostrar_codigo_cuatro_digitos (int codigo) {

    if ((codigo > 999) && (codigo < 10000)) {

        printf("%i", codigo);

    } else if ((codigo > 99) && (codigo < 1000)) {

        printf("0%i", codigo);

    } else if ((codigo > 9) && (codigo < 100)) {

        printf("00%i", codigo);

    } else {

        printf("000%i", codigo);

    }

}

void mostrar (cliente_t cliente) {

    mostrar_codigo_cuatro_digitos(cliente.cod_cli);
    printf(" - %s, %s", cliente.apellido, cliente.nombres);

}

//Si el archivo está vacío, lo crea y crea una tabla_hash vacía para operar, la cual devuelve.
//Al crearla, deja cada código de cliente en 0 (para mostrar que el cliente es nulo).
//¡OJO! Si el archivo ya existe en la misma carpeta, lo destruye y crea uno nuevo vacío.
registro_tabla_t* crear () {

    FILE* archivo;

    archivo = fopen(ruta_archivo, "wb");

    fseek(archivo, 0, SEEK_SET);

    registro_tabla_t* tabla_hash = (registro_tabla_t*) calloc(700, sizeof(registro_tabla_t));

    for (int i = 0; i < 700; i++) { //No se que tan necesario sea dado que utilizo calloc, pero por si las moscas

        tabla_hash[i].cliente.cod_cli = 0;
        tabla_hash[i].tam_colisiones = 0;

        for (int j = 0; j < MAX_COLISIONES_POR_POS; j++) {

            tabla_hash[i].colisiones[j].cod_cli = 0;

        }

        fwrite(&tabla_hash[i], sizeof(registro_tabla_t), 1, archivo);

    }

    fclose(archivo);

    return tabla_hash;
}

//Recupera la tabla hash guardada en el archivo
registro_tabla_t* recuperar () {

    FILE* archivo;

    archivo = fopen(ruta_archivo, "rb+");

    //fseek(archivo, 0, SEEK_SET);

    registro_tabla_t* tabla_hash = (registro_tabla_t*) calloc(700, sizeof(registro_tabla_t));

    for (int i = 0; i < 700; i++) {

        fread(&tabla_hash[i], sizeof(registro_tabla_t), 1, archivo);

    }

    fclose(archivo);

    return tabla_hash;

}

//Sobreescribe el guardado previo con los nuevos datos.
void guardar (registro_tabla_t* tabla_hash) {

    FILE* archivo;

    archivo = fopen(ruta_archivo, "wb+");

    //fseek(archivo, 0, SEEK_SET);

    for (int i = 0; i < 700; i++) {

        fwrite(&tabla_hash[i], sizeof(registro_tabla_t), 1, archivo);

    }

    fclose(archivo);

}

void procesar_alta (registro_tabla_t* tabla_hash) {

    printf("Se le pide ingresar los datos del cliente a dar de alta.\n");
    cliente_t cliente = recibir_cliente();

    alta(tabla_hash, cliente.nombres, cliente.apellido, cliente.cod_cli);

}

void procesar_baja (registro_tabla_t* tabla_hash) {

    printf("Se le pide ingresar el código del cliente a dar de baja.\n");

    int cod_borrar = 0;

    printf("Ingrese el código del cliente[1-4 dígitos]: ");
    scanf("%i", &(cod_borrar));

    while (getchar() != '\n');

    while (!codigo_cliente_valido(cod_borrar)) {

        printf("Código no válido.\n");
        printf("Ingrese el código del cliente: ");
        scanf("%i", &(cod_borrar));

        while (getchar() != '\n');

    }

    baja(tabla_hash, cod_borrar);

}

void procesar_modificar (registro_tabla_t* tabla_hash) {

    printf("Se le pide ingresar los datos del cliente a modificar.\n");
    printf("Ingrese los datos utilizando el mismo codigo de cliente.\n");
    cliente_t cliente = recibir_cliente();

    modificar(tabla_hash, cliente.nombres, cliente.apellido, cliente.cod_cli);

}

void mostrar_tabla (registro_tabla_t* tabla_hash) {

    printf("\n\n********************CLIENTES********************\n\n");

    for (int i = 0; i < 700; i++) {

        if (!cliente_nulo(tabla_hash[i].cliente)) {

            mostrar(tabla_hash[i].cliente);

            if (tabla_hash[i].tam_colisiones > 0) {

                for (int j = 0; j < tabla_hash[i].tam_colisiones; j++) {

                    printf("\n  %i  ", j);
                    mostrar(tabla_hash[i].colisiones[j]);

                }

            }

            printf("\n");

        }

    }

}

void mostrar_menu_tabla () {

    printf("\n\n----------MENU TABLA----------\n");
    printf("1. Alta                       \n");
    printf("2. Baja                       \n");
    printf("3. Modificar                  \n");
    printf("4. Mostrar                    \n");
    printf("0. Salir                      \n");

}

int recibir_opcion (int min, int max) {

    int opcion = 0;

    printf("Ingrese la opcion que desea [%i-%i]: ", min, max);
    scanf("%i", &(opcion));

    while (getchar() != '\n');

    while ((opcion < min) || (opcion > max)) {

        printf("Opción no válida.\n");
        printf("Ingrese la opcion que desea [%i-%i]: ", min, max);
        scanf("%i", &(opcion));

        while (getchar() != '\n');

    }

    return opcion;

}

void procesar_menu_tabla (registro_tabla_t* tabla_hash) {

    int opcion = -1;

    while (opcion != 0) {

        mostrar_menu_tabla();
        opcion = recibir_opcion(0, 4);

        switch (opcion) {

            case 0:
            break;
            case 1: //alta
                procesar_alta(tabla_hash);
            break;
            case 2: //baja
                procesar_baja(tabla_hash);
            break;
            case 3: //modificacion
                procesar_modificar(tabla_hash);
            break;
            case 4:
                mostrar_tabla(tabla_hash);
            break;
            default:
                printf("nada que ver aquí.\n");

        }

    }

    guardar(tabla_hash);

}

void procesar_crear () {

    registro_tabla_t* tabla_hash = crear();

    if (tabla_hash != NULL) {

        procesar_menu_tabla(tabla_hash);

    } else {

        printf("Memoria insuficiente.\n");

    }

    free(tabla_hash);

}

void procesar_recuperar () {

    registro_tabla_t* tabla_hash = recuperar();

    if (tabla_hash != NULL) {

        procesar_menu_tabla(tabla_hash);

    } else {

        printf("Memoria insuficiente.\n");

    }

    free(tabla_hash);

}

void mostrar_menu_archivo () {

    printf("\n\n********************MENU PRINCIPAL********************\n\n");
    printf("1. Crear   (CUIDADO destruye archivo previo si existe)\n");
    printf("2. Recuperar                                          \n");
    printf("0. Salir                                              \n");

}

void procesar_menu_archivo () {

    int opcion = -1;

    while (opcion != 0) {

        mostrar_menu_archivo();
        opcion = recibir_opcion(0, 2);

        switch (opcion) {

            case 0:
            break;
            case 1: //crear
                procesar_crear();
            break;
            case 2: //recuperar
                procesar_recuperar();
            break;
            case 744:
                printf("Este mensaje no está aquí.\n"); //bkp (jp) ea
            default:
                printf("Usted no debería ver este mensaje\n");

        }

    }

    printf("\n\nPrograma terminado.\nSaludos\n");

}

int main () {

    procesar_menu_archivo();

    return 0;
}
