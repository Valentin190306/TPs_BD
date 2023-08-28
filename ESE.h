#ifndef ESTRUCTURAS_SE_H
#define ESTRUCTURAS_SE_H

#include <stdbool.h>
#include "tipo_elemento.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - Listas

struct ListaRep;
typedef struct ListaRep *Lista;

struct IteradorRep;
typedef struct IteradorRep *Iterador;

Lista l_crear ();

bool l_es_vacia (Lista lista);

bool l_es_llena (Lista lista);

int l_cantidad (Lista lista);

void l_agregar (Lista lista, TipoElemento elemento);

void l_borrar (Lista lista, int clave);

TipoElemento l_buscar (Lista lista, int clave);

void l_insertar (Lista lista, TipoElemento elemento, int posicion);

void l_eliminar (Lista lista, int posicion);

TipoElemento l_recuperar (Lista lista, int posicion);

void l_mostrarLista (Lista lista);

Iterador iterador (Lista lista);

bool hay_siguiente (Iterador iterador);

TipoElemento siguiente (Iterador iterador);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - Pilas

struct PilaRep;
typedef struct PilaRep *Pila;

Pila p_crear ();

bool p_es_vacia (Pila pila);

bool p_es_llena (Pila pila);

int p_cantidad (Pila pila);

void p_apilar (Pila pila, TipoElemento elemento);

TipoElemento p_desapilar (Pila pila);

TipoElemento p_tope (Pila pila);

void p_mostrarPila (Pila pila);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - Colas

struct ColaRep;
typedef struct ColaRep *Cola;

Cola c_crear ();

bool c_es_vacia (Cola cola);

bool c_es_llena (Cola cola);

void c_encolar (Cola cola, TipoElemento elemento);

TipoElemento c_desencolar (Cola cola);

TipoElemento c_recuperar (Cola cola);

void c_mostrarCola (Cola cola);

#endif