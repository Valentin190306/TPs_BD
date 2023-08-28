#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "Validacion.h"

int EntradaEntera(char buffer[], int nroNat, int LimInferior, int LimSuperior) {
    int valido, numero, i;
    int nneg;
    char numeros[11] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-'};

    do {
        i = 0; nneg = 0;
        buffer[strcspn(buffer, "\n")] = '\0';
        valido = 1;
        if (strchr(buffer, ',') != NULL || strchr(buffer, '.') != NULL) {
            valido = 0;
            printf("\n%d <! Entrada invalida (decimal)", __LINE__);
        }

        do {
            if (strchr(numeros, buffer[i]) == NULL) {
                valido = 0;
                printf("\n%d <! Entrada invalida (alfabetico)", __LINE__);
            }
            i++;
        } while (buffer[i] != '\0' && valido == 1);
        
        if (strchr(buffer, '-') != NULL) nneg = 1;

        //printf("\n >! Contenido del buffer: %s", buffer);
        
        sscanf(buffer, "%d", &numero);

        //printf("\n >! Valor en transito: %d", numero);
        if (valido == 1) {
            if (nroNat == 1 && numero <= 0) {
                valido = 0;
                printf("\n%d <! Entrada invalida (valor no natural)", __LINE__);
            }
            else if (numero > LimSuperior && LimSuperior != LimInferior) {
                valido = 0;
                printf("\n%d <! Entrada invalida (valor fuera de rango)", __LINE__);
            }
            else if (numero < LimInferior && LimSuperior != LimInferior) {
                valido = 0;
                printf("\n%d <! Entrada invalida (valor fuera de rango)", __LINE__);
            }
        }

        if (valido == 0) {
            printf("\n%d << Ingrese un valor valido: ", __LINE__);
            fgets(buffer, 100, stdin);
        }
    } while (valido == 0);

    fflush(stdin);
    return numero;
}

int CadenaValida(char entrada[],  char permitidos[], char prohibidos[]) {
    int valido = 1;

    if ((permitidos == NULL && prohibidos == NULL) || (permitidos != NULL && prohibidos != NULL)) {
        printf ("\n%d >! ERROR de sintaxis, depurar . . . ", __LINE__);
        return -1;
    }

    else if (prohibidos == NULL) {
        entrada[strcspn(entrada, "\n")] = '\0';
        for (int i = 0; entrada[i] != '\0' && valido != 0; i++) {
            entrada[i] = toupper(entrada[i]);
            if (strrchr(permitidos,  entrada[i]) == NULL) {
                printf("\n%d >! Cadena invalida . . . ", __LINE__);
                valido = 0;
                }
        }
    }

    else if (permitidos == NULL) {
        entrada[strcspn(entrada, "\n")] = '\0';
        for (int i = 0; entrada[i] != '\0' && valido != 0; i++) {
            entrada[i] = toupper(entrada[i]);
            if (strrchr(prohibidos,  entrada[i]) != NULL) {
                printf("\n%d >! Cadena invalida . . . ", __LINE__);
                valido = 0;
                }
        }
    }

    return valido;
}