#include "lista.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define INVALID_ARGUMENT 0

typedef struct nodo {
	void *elemento;
	struct nodo *siguiente;
} nodo_t;

struct lista_iterador {
	nodo_t *corriente;
	lista_t *lista;
};

struct lista {
	nodo_t *nodo_inicio;
	nodo_t *nodo_final;
	size_t tope;
};

lista_t *lista_crear()
{
	lista_t *nueva_lista = malloc(sizeof(lista_t));

	if (nueva_lista == NULL)
		return NULL;

	nueva_lista->nodo_inicio = NULL;
	nueva_lista->nodo_final = NULL;

	nueva_lista->tope = VACIO;

	return nueva_lista;
}

lista_t *lista_insertar(lista_t *lista, void *elemento)
{
	if ((lista == NULL))
		return NULL;

	nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));

	if (nuevo_nodo == NULL) {
		lista_destruir_todo(lista, free);
		return NULL;
	}

	nuevo_nodo->elemento = elemento;
	nuevo_nodo->siguiente = NULL;

	if (lista->tope == VACIO) {
		lista->nodo_inicio = nuevo_nodo;
		lista->nodo_final = nuevo_nodo;
		lista->tope++;

		return lista;
	}

	lista->nodo_final->siguiente = nuevo_nodo;
	lista->nodo_final = nuevo_nodo;

	lista->tope++;

	return lista;
}

lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if ((lista == NULL))
		return NULL;

	if ((posicion > lista->tope) || (lista->tope == VACIO) ||
	    posicion == lista->tope) {
		return lista_insertar(lista, elemento);
	}

	nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));

	if (nuevo_nodo == NULL) {
		lista_destruir_todo(lista, NULL);
		return NULL;
	}

	nuevo_nodo->elemento = elemento;

	if (posicion == 0) {
		nuevo_nodo->siguiente = lista->nodo_inicio;
		lista->nodo_inicio = nuevo_nodo;
		lista->tope++;

		return lista;
	}

	size_t i;
	nodo_t *nodo_auxiliar;

	nodo_auxiliar = lista->nodo_inicio;

	for (i = 0; i < posicion - 1; i++) {
		nodo_auxiliar = nodo_auxiliar->siguiente;
	}

	nuevo_nodo->siguiente = nodo_auxiliar->siguiente;
	nodo_auxiliar->siguiente = nuevo_nodo;

	lista->tope++;

	return lista;
}

void *lista_quitar(lista_t *lista)
{
	if ((lista == NULL) || (lista->tope == VACIO))
		return NULL;

	void *elemento_auxiliar;
	nodo_t *nodo_aux;
	nodo_aux = lista->nodo_inicio;

	if (lista->tope == 1) {
		elemento_auxiliar = lista->nodo_inicio->elemento;
		free(lista->nodo_inicio);
		lista->nodo_inicio = NULL;
		lista->nodo_final = NULL;
		lista->tope--;

		return elemento_auxiliar;
	}

	while (nodo_aux->siguiente->siguiente != NULL)
		nodo_aux = nodo_aux->siguiente;

	elemento_auxiliar = nodo_aux->siguiente->elemento;
	free(nodo_aux->siguiente);
	nodo_aux->siguiente = NULL;

	lista->nodo_final = nodo_aux;
	lista->tope--;

	return elemento_auxiliar;
}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	if ((lista == NULL) || (lista->tope == VACIO))
		return NULL;
	if ((posicion > lista->tope) || (posicion == lista->tope))
		return lista_quitar(lista);

	nodo_t *nodo_auxiliar;
	nodo_t *nodo_a_eliminar;
	void *elemento_auxiliar;
	size_t posicion_iteracion;

	nodo_auxiliar = lista->nodo_inicio;

	if (posicion == 0) {
		elemento_auxiliar = nodo_auxiliar->elemento;
		lista->nodo_inicio = lista->nodo_inicio->siguiente;
		free(nodo_auxiliar);
		lista->tope--;

		return elemento_auxiliar;
	}

	for (posicion_iteracion = 0; posicion_iteracion < posicion - 1;
	     posicion_iteracion++) {
		nodo_auxiliar = nodo_auxiliar->siguiente;
	}

	nodo_a_eliminar = nodo_auxiliar->siguiente;
	nodo_auxiliar->siguiente = nodo_auxiliar->siguiente->siguiente;

	elemento_auxiliar = nodo_a_eliminar->elemento;

	free(nodo_a_eliminar);

	lista->tope--;

	return elemento_auxiliar;
}

void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if ((lista == NULL) || (posicion >= lista->tope))
		return NULL;

	nodo_t *nodo_auxiliar;
	size_t posicion_iteracion = 0;

	nodo_auxiliar = lista->nodo_inicio;

	while ((posicion_iteracion < posicion)) {
		nodo_auxiliar = nodo_auxiliar->siguiente;
		posicion_iteracion++;
	}

	return nodo_auxiliar->elemento;
}

void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{
	if ((lista == NULL) || (lista->tope == VACIO) || (comparador == NULL))
		return NULL;

	nodo_t *nodo_auxiliar = lista->nodo_inicio;

	while ((comparador(nodo_auxiliar->elemento, contexto) != 0) &&
	       (nodo_auxiliar->siguiente != NULL)) {
		nodo_auxiliar = nodo_auxiliar->siguiente;
	}

	if (comparador(nodo_auxiliar->elemento, contexto) == 0)
		return nodo_auxiliar->elemento;

	return NULL;
}

void *lista_primero(lista_t *lista)
{
	if (lista == NULL || (lista->nodo_inicio == NULL))
		return NULL;
	return lista->nodo_inicio->elemento;
}

void *lista_ultimo(lista_t *lista)
{
	if (lista == NULL || lista->tope == VACIO)
		return NULL;
	return lista->nodo_final->elemento;
}

bool lista_vacia(lista_t *lista)
{
	if ((lista == NULL) || (lista->tope == VACIO))
		return true;
	return false;
}

size_t lista_tamanio(lista_t *lista)
{
	if (lista == NULL)
		return VACIO;
	return lista->tope;
}

void lista_destruir(lista_t *lista)
{
	if (lista == NULL)
		return;

	nodo_t *nodo_auxiliar;

	while (lista->nodo_inicio != NULL) {
		nodo_auxiliar = lista->nodo_inicio->siguiente;
		free(lista->nodo_inicio);
		lista->nodo_inicio = nodo_auxiliar;
	}
	free(lista);
	return;
}

void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if (lista == NULL)
		return;
	nodo_t *nodo_auxiliar;

	nodo_auxiliar = lista->nodo_inicio;

	while (nodo_auxiliar != NULL && funcion != NULL) {
		funcion(nodo_auxiliar->elemento);
		nodo_auxiliar = nodo_auxiliar->siguiente;
	}

	lista_destruir(lista);

	return;
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (lista == NULL)
		return NULL;

	lista_iterador_t *nuevo_iterador = malloc(sizeof(lista_iterador_t));

	if (nuevo_iterador == NULL)
		return NULL;

	nuevo_iterador->corriente = lista->nodo_inicio;
	nuevo_iterador->lista = lista;

	return nuevo_iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	if (iterador == NULL || iterador->corriente == NULL)
		return false;
	return true;
}

bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if (iterador == NULL || iterador->corriente == NULL)
		return false;

	iterador->corriente = iterador->corriente->siguiente;

	if (iterador->corriente == NULL)
		return false;
	return true;
}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if ((iterador == NULL) || (iterador->corriente == NULL))
		return NULL;
	return iterador->corriente->elemento;
}

void lista_iterador_destruir(lista_iterador_t *iterador)
{
	free(iterador);
	return;
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
			       void *contexto)
{
	if ((lista == NULL) || (funcion == NULL) || (lista->tope == 0))
		return INVALID_ARGUMENT;

	nodo_t *nodo_auxiliar;
	size_t elementos_iterados = 1;

	nodo_auxiliar = lista->nodo_inicio;

	while ((funcion(nodo_auxiliar->elemento, contexto)) &&
	       nodo_auxiliar->siguiente != NULL) {
		elementos_iterados++;
		nodo_auxiliar = nodo_auxiliar->siguiente;
	}

	return elementos_iterados;
}
