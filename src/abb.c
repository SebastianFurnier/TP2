#include "abb.h"
#include "abb_estructura_privada.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#define VACIO 0

abb_t *abb_crear(abb_comparador comparador)
{
	if (!comparador)
		return NULL;

	abb_t *arbol = malloc(sizeof(abb_t));

	if (!arbol)
		return NULL;

	arbol->comparador = comparador;
	arbol->nodo_raiz = NULL;
	arbol->tamanio = 0;

	return arbol;
}

nodo_abb_t *crear_nodo(void *elemento)
{
	nodo_abb_t *nodo = malloc(sizeof(nodo_abb_t));

	if (!nodo)
		return NULL;
	nodo->derecha = NULL;
	nodo->izquierda = NULL;
	nodo->elemento = elemento;

	return nodo;
}

nodo_abb_t *abb_insertar_recursivo(nodo_abb_t *raiz, void *elemento,
				   abb_comparador comparador)
{
	if (!raiz)
		return crear_nodo(elemento);

	int comparacion = comparador(elemento, raiz->elemento);

	if (comparacion <= 0)
		raiz->izquierda = abb_insertar_recursivo(raiz->izquierda,
							 elemento, comparador);
	else if (comparacion > 0) {
		raiz->derecha = abb_insertar_recursivo(raiz->derecha, elemento,
						       comparador);
	}

	return raiz;
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if (!arbol)
		return NULL;

	arbol->nodo_raiz = abb_insertar_recursivo(arbol->nodo_raiz, elemento,
						  arbol->comparador);
	arbol->tamanio++;

	return arbol;
}

void *elemento_mas_derecho(nodo_abb_t *raiz, void **elemento)
{
	if (!raiz->derecha) {
		*elemento = raiz->elemento;
		nodo_abb_t *nodo_izquierdo = raiz->izquierda;
		free(raiz);
		return nodo_izquierdo;
	}
	raiz->derecha = elemento_mas_derecho(raiz->derecha, elemento);

	return raiz;
}

void *abb_quitar_recursivo(nodo_abb_t *raiz, void *elemento,
			   abb_comparador comparador, void **extraido)
{
	if (!raiz)
		return NULL;

	int comparacion = comparador(elemento, raiz->elemento);

	if (comparacion == 0) {
		nodo_abb_t *nodo_izquierdo = raiz->izquierda;
		nodo_abb_t *nodo_derecho = raiz->derecha;
		*extraido = raiz->elemento;

		if (nodo_izquierdo && nodo_derecho) {
			void *elemento_derecho = NULL;
			raiz->izquierda = elemento_mas_derecho(
				raiz->izquierda, &elemento_derecho);
			raiz->elemento = elemento_derecho;
			return raiz;
		} else {
			free(raiz);
			if (!nodo_izquierdo)
				return nodo_derecho;
			return nodo_izquierdo;
		}
	} else if (comparacion < 0)
		raiz->izquierda = abb_quitar_recursivo(
			raiz->izquierda, elemento, comparador, extraido);
	else
		raiz->derecha = abb_quitar_recursivo(raiz->derecha, elemento,
						     comparador, extraido);

	return raiz;
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	if (!arbol || !arbol->nodo_raiz)
		return NULL;
	void *extraido = NULL;
	arbol->nodo_raiz = abb_quitar_recursivo(arbol->nodo_raiz, elemento,
						arbol->comparador, &extraido);

	if (extraido != NULL)
		arbol->tamanio--;

	return extraido;
}

void *abb_buscar_recursivo(nodo_abb_t *raiz, void *elemento,
			   abb_comparador comparador, void **elemento_aux)
{
	if (!raiz)
		return NULL;

	elemento_aux = raiz->elemento;
	int comparacion = comparador(elemento, elemento_aux);

	if (comparacion < 0)
		elemento_aux = abb_buscar_recursivo(raiz->izquierda, elemento,
						    comparador, elemento_aux);
	else if (comparacion > 0)
		elemento_aux = abb_buscar_recursivo(raiz->derecha, elemento,
						    comparador, elemento_aux);

	return elemento_aux;
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	if (!arbol || !arbol->nodo_raiz)
		return NULL;
	void **elemento_aux = NULL;
	
	elemento_aux = abb_buscar_recursivo(
		arbol->nodo_raiz, elemento, arbol->comparador, elemento_aux);
	return elemento_aux;
}

bool abb_vacio(abb_t *arbol)
{
	if (!arbol || !arbol->nodo_raiz)
		return true;
	return false;
}

size_t abb_tamanio(abb_t *arbol)
{
	if (!arbol)
		return VACIO;
	return arbol->tamanio;
}

void abb_destruir_todo_recursivo(nodo_abb_t *raiz, void (*destructor)(void *))
{
	if (!raiz)
		return;
	abb_destruir_todo_recursivo(raiz->izquierda, destructor);
	if (destructor != NULL)
		destructor(raiz->elemento);
	abb_destruir_todo_recursivo(raiz->derecha, destructor);
	free(raiz);
	return;
}

void abb_destruir(abb_t *arbol)
{
	if (!arbol)
		return;
	abb_destruir_todo_recursivo(arbol->nodo_raiz, NULL);
	free(arbol);
	return;
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if (!arbol)
		return;
	abb_destruir_todo_recursivo(arbol->nodo_raiz, destructor);
	free(arbol);

	return;
}

bool evaluar_continuar(nodo_abb_t *raiz, void *aux, size_t *invocaciones, bool (*funcion)(void *, void *))
{
	if (!raiz || !funcion)
		return false;
	(*invocaciones)++;
	return funcion(raiz->elemento, aux);
}

void abb_con_cada_elemento_postorden(nodo_abb_t *raiz,
				     bool (*funcion)(void *, void *), void *aux,
				     size_t *invocaciones, bool *continuar,
				     abb_recorrido recorrido)
{
	if (!(*continuar) || !raiz)
		return;

	abb_con_cada_elemento_postorden(raiz->izquierda, funcion, aux,
					invocaciones, continuar, recorrido);

	abb_con_cada_elemento_postorden(raiz->derecha, funcion, aux,
					invocaciones, continuar, recorrido);

	(*continuar) = evaluar_continuar(raiz, aux, invocaciones, funcion);

	return;
}

void abb_con_cada_elemento_preorden(nodo_abb_t *raiz,
				    bool (*funcion)(void *, void *), void *aux,
				    size_t *invocaciones, bool *continuar,
				    abb_recorrido recorrido)
{
	if (!(*continuar) || !raiz)
		return;

	(*continuar) = evaluar_continuar(raiz, aux, invocaciones, funcion);

	abb_con_cada_elemento_preorden(raiz->izquierda, funcion, aux,
				       invocaciones, continuar, recorrido);

	abb_con_cada_elemento_preorden(raiz->derecha, funcion, aux,
				       invocaciones, continuar, recorrido);

	return;
}

void abb_con_cada_elemento_inorden(nodo_abb_t *raiz,
				   bool (*funcion)(void *, void *), void *aux,
				   size_t *invocaciones, bool *continuar,
				   abb_recorrido recorrido)
{
	if (!(*continuar) || !raiz)
		return;

	abb_con_cada_elemento_inorden(raiz->izquierda, funcion, aux,
				      invocaciones, continuar, recorrido);

	(*continuar) = evaluar_continuar(raiz, aux, invocaciones, funcion);

	abb_con_cada_elemento_inorden(raiz->derecha, funcion, aux, invocaciones,
				      continuar, recorrido);

	return;
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	if (!arbol || !funcion)
		return VACIO;

	size_t cantidad_invocaciones = 0;
	bool continuar = true;

	if (recorrido == INORDEN)
		abb_con_cada_elemento_inorden(arbol->nodo_raiz, funcion, aux,
					      &cantidad_invocaciones,
					      &continuar, recorrido);
	else if (recorrido == PREORDEN)
		abb_con_cada_elemento_preorden(arbol->nodo_raiz, funcion, aux,
					       &cantidad_invocaciones,
					       &continuar, recorrido);
	else if (recorrido == POSTORDEN)
		abb_con_cada_elemento_postorden(arbol->nodo_raiz, funcion, aux,
						&cantidad_invocaciones,
						&continuar, recorrido);

	return cantidad_invocaciones;
}

void guardar_en_arreglo(nodo_abb_t *raiz, void **array, size_t tamanio_array,
			size_t *elementos_guardados)
{
	if ((*elementos_guardados) < tamanio_array) {
		array[(*elementos_guardados)] = raiz->elemento;
		(*elementos_guardados)++;
	}
}

void abb_recorrer_postorden(nodo_abb_t *raiz, void **array,
			    size_t tamanio_array, size_t *elementos_guardados,
			    abb_recorrido recorrido)
{
	if (!raiz)
		return;

	abb_recorrer_postorden(raiz->izquierda, array, tamanio_array,
			       elementos_guardados, recorrido);

	abb_recorrer_postorden(raiz->derecha, array, tamanio_array,
			       elementos_guardados, recorrido);

	guardar_en_arreglo(raiz, array, tamanio_array, elementos_guardados);

	return;
}

void abb_recorrer_preorden(nodo_abb_t *raiz, void **array, size_t tamanio_array,
			   size_t *elementos_guardados, abb_recorrido recorrido)
{
	if (!raiz)
		return;

	guardar_en_arreglo(raiz, array, tamanio_array, elementos_guardados);

	abb_recorrer_preorden(raiz->izquierda, array, tamanio_array,
			      elementos_guardados, recorrido);

	abb_recorrer_preorden(raiz->derecha, array, tamanio_array,
			      elementos_guardados, recorrido);

	return;
}

void abb_recorrer_inorden(nodo_abb_t *raiz, void **array, size_t tamanio_array,
			  size_t *elementos_guardados, abb_recorrido recorrido)
{
	if (!raiz)
		return;

	abb_recorrer_inorden(raiz->izquierda, array, tamanio_array,
			     elementos_guardados, recorrido);

	guardar_en_arreglo(raiz, array, tamanio_array, elementos_guardados);

	abb_recorrer_inorden(raiz->derecha, array, tamanio_array,
			     elementos_guardados, recorrido);

	return;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	if (!arbol)
		return VACIO;

	size_t elementos_guardados = 0;

	if (recorrido == INORDEN)
		abb_recorrer_inorden(arbol->nodo_raiz, array, tamanio_array,
				     &elementos_guardados, recorrido);
	else if (recorrido == PREORDEN)
		abb_recorrer_preorden(arbol->nodo_raiz, array, tamanio_array,
				      &elementos_guardados, recorrido);
	else if (recorrido == POSTORDEN)
		abb_recorrer_postorden(arbol->nodo_raiz, array, tamanio_array,
				       &elementos_guardados, recorrido);

	return elementos_guardados;
}