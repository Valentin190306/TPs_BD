#include "ESE.h"

#include <stdio.h>
#include <stdlib.h>

#define MAXIMA_CANTIDAD 100

#ifndef ESTRUCTURAS_SE_PUNTEROS_C
#define ESTRUCTURAS_SE_PUNTEROS_C

struct Nodo {
    TipoElemento datos;
    struct Nodo *siguiente;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - Listas

struct ListaRep {
    struct Nodo *primero;
    int cantidad;
};

struct IteradorRep {
    struct Nodo *nodo_actual;
};

Lista l_crear () {
    Lista nueva_lista = (Lista) malloc(sizeof(struct ListaRep));
    nueva_lista->primero = NULL;
    nueva_lista->cantidad = 0;
    return nueva_lista;
}

bool l_es_vacia (Lista lista) {
    return lista->cantidad == 0;
}

bool l_es_llena (Lista lista) {
    return lista->cantidad == MAXIMA_CANTIDAD;
}

int l_cantidad (Lista lista) {
    return lista->cantidad;
}

void l_agregar (Lista lista, TipoElemento elemento) {
    if (l_es_llena(lista)) return;

    struct Nodo *nuevo_nodo = malloc(sizeof(struct Nodo));
    nuevo_nodo->datos = elemento;
    nuevo_nodo->siguiente = NULL;

    if (l_es_vacia(lista)) lista->primero = nuevo_nodo;

    else {
        struct Nodo *naux = lista->primero;
        while (naux->siguiente != NULL) naux = naux->siguiente;
        naux->siguiente = nuevo_nodo;
    }

    lista->cantidad++;
}

void l_borrar (Lista lista, int clave) {
    if (l_es_vacia(lista)) return;

    struct Nodo *naux1 = lista->primero;
    
    while (naux1 != NULL && naux1->datos->clave == clave) {
        lista->primero = naux1->siguiente;
        free(naux1);
        naux1 = lista->primero;
        lista->cantidad--;
    }

    while (naux1 != NULL && naux1->siguiente != NULL) {

        if (naux1->siguiente->datos->clave == clave) {
            struct Nodo *naux2 = naux1->siguiente;
            naux1->siguiente = naux2->siguiente;
            free(naux2);
            lista->cantidad--;
        }

        else naux1 = naux1->siguiente;
    }
}

TipoElemento l_buscar (Lista lista, int clave) {
    struct Nodo *naux = lista->primero;

    while (naux != NULL) {
        if (naux->datos->clave == clave) return naux;
        naux = naux->siguiente;
    }
    
    return NULL;
}

void l_insertar (Lista lista, TipoElemento elemento, int posicion) {
    if (l_es_llena(lista)) return;

    struct Nodo *nuevo_nodo = malloc(sizeof(struct Nodo));
    nuevo_nodo->datos = elemento;
    nuevo_nodo->siguiente = NULL;

    if (posicion == 1) {
        nuevo_nodo->siguiente = lista->primero;
        lista->primero = nuevo_nodo;
    }

    else {
        struct Nodo *naux = lista->primero;
        for (int i = 0 ; i < posicion - 2 ; i++) naux = naux->siguiente;
        nuevo_nodo->siguiente = naux->siguiente;
        naux->siguiente = nuevo_nodo;
    }

    lista->cantidad++;
}

void l_eliminar (Lista lista, int posicion) {
    if (l_es_vacia(lista)) return;

    struct Nodo *naux1 = lista->primero;

    if (posicion >= 1 && posicion <= l_cantidad(lista)) {
        
        if (posicion == 1) {
            lista->primero = naux1->siguiente;
            free(naux1);
        }

        else {
            for (int i = 0 ; i < posicion - 2 ; i++) naux1 = naux1->siguiente;
            struct Nodo *naux2 = naux1->siguiente;
            naux1->siguiente = naux2->siguiente;
            free(naux2);
        }

        lista->cantidad--;
    }
}

TipoElemento l_recuperar (Lista lista, int posicion) {
    if (l_es_vacia(lista)) return NULL;

    struct Nodo *naux = lista->primero;
    for (int i = 0 ; i < posicion - 2 ; i++) naux = naux->siguiente;
    return naux->datos;
}

void l_mostrarLista(Lista lista) {
    if (l_es_vacia(lista)) {
        printf("\n/> Lista vacia . . .\n");
        return;
    }

    printf("\n/> Nodos de la lista\n__ Nodo - Clave - Valor (Dir)");
    struct Nodo *naux = lista->primero;
    int i = 0;

    while (naux != NULL) {
        printf("\n__ %d - %d - %p", i+1, naux->datos->clave, naux->datos->valor);
        naux = naux->siguiente;
    }

    printf("\n");
}

Iterador iterador (Lista lista) {
    Iterador iter = (Iterador) malloc(sizeof(struct IteradorRep));
    iter->nodo_actual = lista->primero;
    return iter;
}

bool hay_siguiente (Iterador iterador) {
    return iterador->nodo_actual != NULL;
}

TipoElemento siguiente (Iterador iterador) {
    TipoElemento taux = iterador->nodo_actual->datos;
    iterador->nodo_actual = iterador->nodo_actual->siguiente;
    return taux;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - Pilas

struct PilaRep {
    struct Nodo *tope;
    int cantidad;
};

Pila p_crear () {
    Pila nueva_pila = (Pila) malloc(sizeof(struct PilaRep));
    nueva_pila->tope = NULL;
    nueva_pila->cantidad = 0;
    return nueva_pila;
}

bool p_es_vacia (Pila pila) {
    return pila->cantidad == 0;
}

bool p_es_llena (Pila pila) {
    return pila->cantidad == MAXIMA_CANTIDAD;
}

int p_cantidad (Pila pila) {
    return pila->cantidad;
}

void p_apilar (Pila pila, TipoElemento elemento) {
    if (p_es_llena(pila)) return;
    struct Nodo *nuevo_nodo = malloc(sizeof(struct Nodo));
    nuevo_nodo->datos = elemento;
    nuevo_nodo->siguiente = pila->tope;
    pila->tope = nuevo_nodo;
    pila->cantidad++;
}

TipoElemento p_desapilar (Pila pila) {
    if (pila->tope == NULL) return NULL;
    struct Nodo *naux = pila->tope;
    TipoElemento elemento = naux->datos;
    pila->tope = naux->siguiente;
    pila->cantidad--;
    free(naux);
    return elemento;
}

TipoElemento p_tope (Pila pila) {
    TipoElemento elemento = pila->tope->datos;
    return elemento;
}

void p_mostrarPila (Pila pila) {
    if (p_es_vacia(pila)) {
        printf("\n/> Pila vacia . . .\n");
        return;
    }

    printf("\n/> Nodos de la pila\n__ Nodo - Clave - Valor (Dir)");
    TipoElemento taux;
    Pila paux = p_crear();
    int i = 0;    

    while (!p_es_vacia(pila)) {
        taux = p_desapilar(pila);
        printf("\n__ %d - %d - %p", i+1, taux->clave, taux->valor);
        p_apilar(paux, taux);
    }

    while (!p_es_vacia(paux)) p_apilar(pila, p_desapilar(paux));
    
    printf("\n");
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - Colas

struct ColaRep {
    struct Nodo *primero;
    struct Nodo *ultimo;
    int cantidad;
};

Cola c_crear () {
    Cola nueva_cola = (Cola) malloc(sizeof(struct ColaRep));
    nueva_cola->primero = NULL;
    nueva_cola->ultimo = NULL;
    nueva_cola->cantidad = 0;
    return nueva_cola;
}

bool c_es_vacia (Cola cola) {
    return cola->primero == NULL;
}

bool c_es_llena (Cola cola) {
    return cola->cantidad == MAXIMA_CANTIDAD; 
}

void c_encolar (Cola cola, TipoElemento elemento) {
    if (c_es_llena(cola)) return;

    struct Nodo *nuevo_nodo = malloc(sizeof(struct Nodo));
    nuevo_nodo->datos = elemento;
    nuevo_nodo->siguiente = NULL;

    if (c_es_vacia(cola)) cola->primero = nuevo_nodo;
    else                  cola->ultimo->siguiente = nuevo_nodo;

    cola->ultimo = nuevo_nodo;
    cola->cantidad++;
}

TipoElemento c_desencolar(Cola cola) {
    if (c_es_vacia(cola)) return NULL;
    struct Nodo *naux = cola->primero;
    TipoElemento elemento = naux->datos;
    cola->primero = naux->siguiente;
    cola->cantidad--;
    free(naux);
    return elemento;
}

TipoElemento c_recuperar (Cola cola) {
    if (c_es_vacia(cola)) return NULL;
    TipoElemento elemento = cola->primero->datos;
    return elemento;
}

void c_mostrarCola (Cola cola) {
    if (c_es_vacia(cola)) {
        printf("\n/> Cola vacia . . .\n");
        return;
    }

    printf("\n/> Nodos de la cola\n__ Nodo - Clave - Valor (Dir)");
    struct Nodo *naux = cola->primero;
    int i = 0;
    
    while (naux != NULL) {
        printf("\n__ %d - %d - %p", i+1, naux->datos->clave, naux->datos->valor);
        naux = naux->siguiente;
    }

    printf("\n");
}

#endif