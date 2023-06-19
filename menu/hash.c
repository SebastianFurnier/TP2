#include <string.h>
#include <stdlib.h>

#include "hash.h"

#define FACTOR_CARGA_MAXIMO 0.7
#define TAMANIO_MINIMO 3

typedef struct nodo {
	char *clave;
	void *valor;
	struct nodo *siguiente;
} nodo_t;

struct hash {
	size_t capacidad;
	size_t cantidad;
	nodo_t **vector;
};
/*
*Esta funcion recibe un string y nos devuelve una clave numerica unica que caracteriza dicho string.
*/
size_t funcion_hash(const char *str)
{
	size_t hash = 5381;
	int c;

	while ((c = *str++)) {
		hash = ((hash << 5) + hash) + (size_t)c; // hash * 33 + c
	}
	return hash;
}

hash_t *hash_crear(size_t capacidad)
{
	if (capacidad < TAMANIO_MINIMO)
		capacidad = TAMANIO_MINIMO;

	hash_t *hash = malloc(sizeof(hash_t));

	if (!hash)
		return NULL;

	hash->capacidad = capacidad;
	hash->cantidad = 0;
	hash->vector = calloc(1, sizeof(nodo_t *) * capacidad);

	return hash;
}

/*
*Crear nodo es una funcion auxiliar usada por la primitiva hash_insertar.
*Recibe la clave y el valor pasadas por el usuario y devuelve una estructura de tipo nodo_t
*que luego sera insertada en el diccionario.*/
nodo_t *crear_nodo(const char *clave, void *elemento)
{
	nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));

	if (!nuevo_nodo)
		return NULL;

	size_t i = strlen(clave);
	char *nueva_clave = malloc(i + 1);

	if (!nueva_clave)
		return NULL;
	strcpy(nueva_clave, clave);

	nuevo_nodo->clave = nueva_clave;
	nuevo_nodo->siguiente = NULL;
	nuevo_nodo->valor = elemento;

	return nuevo_nodo;
}

/*
*Rehash recibe el hash con el que estamos trabajando, guarda un puntero al vector donde se alojan los pares
*claves valor y crea un nuevo vector de el doble de tamaño al usado por el hash. Luego inserta los pares claves
*valor en el nuevo vector, se lo asigna al hash y destruye los nodos y el vector viejo. 
*/
hash_t *rehash(hash_t *hash)
{
	nodo_t **vector_viejo = hash->vector;
	size_t capacidad_vieja = hash->capacidad;

	hash->vector = calloc(1, sizeof(nodo_t *) * capacidad_vieja * 2);

	if (!hash->vector)
		return NULL;

	hash->capacidad = capacidad_vieja * 2;
	hash->cantidad = 0;

	size_t i;
	nodo_t *nodo_actual;
	nodo_t *nodo_eliminar;

	for (i = 0; i < capacidad_vieja; i++) {
		nodo_actual = vector_viejo[i];
		while (nodo_actual != NULL) {
			hash_insertar(hash, nodo_actual->clave,
				      nodo_actual->valor, NULL);
			nodo_eliminar = nodo_actual;
			nodo_actual = nodo_actual->siguiente;
			free(nodo_eliminar->clave);
			free(nodo_eliminar);
		}
	}
	free(vector_viejo);
	return hash;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if (!hash || !clave)
		return NULL;

	float carga = (float)(hash->cantidad) / (float)(hash->capacidad);

	if (carga > FACTOR_CARGA_MAXIMO)
		hash = rehash(hash);
	if (!hash)
		return NULL;

	size_t posicion = funcion_hash(clave) % hash->capacidad;
	nodo_t *nodo_actual = hash->vector[posicion];

	if (nodo_actual == NULL) {
		hash->vector[posicion] = crear_nodo(clave, elemento);
		hash->cantidad++;
		if (anterior != NULL)
			*anterior = NULL;
		return hash;
	}

	nodo_t *nodo_anterior;

	while (nodo_actual != NULL) {
		if (strcmp(nodo_actual->clave, clave) == 0) {
			if (anterior != NULL)
				*anterior = nodo_actual->valor;
			nodo_actual->valor = elemento;
			return hash;
		}
		nodo_anterior = nodo_actual;
		nodo_actual = nodo_actual->siguiente;
	}

	nodo_t *nuevo_nodo = crear_nodo(clave, elemento);

	if (!nuevo_nodo)
		return NULL;

	nodo_anterior->siguiente = nuevo_nodo;
	hash->cantidad++;

	return hash;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;

	void *elemento_aux = NULL;
	nodo_t *nodo_aux;
	size_t posicion = funcion_hash(clave) % hash->capacidad;

	nodo_aux = hash->vector[posicion];

	if (nodo_aux == NULL)
		return NULL;

	if (strcmp(nodo_aux->clave, clave) == 0) {
		hash->vector[posicion] = nodo_aux->siguiente;
		elemento_aux = nodo_aux->valor;
		free(nodo_aux->clave);
		free(nodo_aux);
		hash->cantidad--;
		return elemento_aux;
	}

	nodo_t *nodo_a_eliminar;

	while (nodo_aux != NULL) {
		if (strcmp(nodo_aux->siguiente->clave, clave) == 0) {
			nodo_a_eliminar = nodo_aux->siguiente;
			nodo_aux->siguiente = nodo_aux->siguiente->siguiente;
			elemento_aux = nodo_a_eliminar->valor;
			free(nodo_a_eliminar->clave);
			free(nodo_a_eliminar);
			hash->cantidad--;
			return elemento_aux;
		}
		nodo_aux = nodo_aux->siguiente;
	}
	return elemento_aux;
}

/*
*Funcion auxiliar usada por hash_obtener y hash_contiene. Busca en el hash la clave pasada por el usuario
*y devuelve el nodo en el que se encuentra.*/
nodo_t *buscar_clave(nodo_t *nodo_actual, const char *clave)
{
	while (nodo_actual != NULL) {
		if (strcmp(nodo_actual->clave, clave) == 0)
			return nodo_actual;
		nodo_actual = nodo_actual->siguiente;
	}
	return NULL;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;

	size_t posicion = funcion_hash(clave) % hash->capacidad;
	nodo_t *nodo_actual = hash->vector[posicion];

	nodo_actual = buscar_clave(nodo_actual, clave);

	if (!nodo_actual)
		return NULL;

	return nodo_actual->valor;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return false;

	size_t posicion = funcion_hash(clave) % hash->capacidad;
	nodo_t *nodo_actual = hash->vector[posicion];

	nodo_actual = buscar_clave(nodo_actual, clave);

	if (!nodo_actual)
		return false;

	return true;
}

size_t hash_cantidad(hash_t *hash)
{
	if (!hash)
		return 0;
	return hash->cantidad;
}

void hash_destruir(hash_t *hash)
{
	if (!hash)
		return;
	hash_destruir_todo(hash, NULL);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash)
		return;

	size_t posicion;
	nodo_t *nodo_actual;
	nodo_t *nodo_siguiente;

	for (posicion = 0; posicion < hash->capacidad; posicion++) {
		nodo_actual = hash->vector[posicion];

		while (nodo_actual != NULL) {
			nodo_siguiente = nodo_actual->siguiente;
			if (destructor != NULL)
				destructor(nodo_actual->valor);
			free(nodo_actual->clave);
			free(nodo_actual);
			nodo_actual = nodo_siguiente;
		}
	}
	free(hash->vector);
	free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux),
			   void *aux)
{
	size_t recorridos = 0;

	if (!hash || !f)
		return recorridos;

	int posicion = 0;
	bool continuar = true;

	nodo_t *nodo_actual;

	while ((recorridos < hash->cantidad) && continuar) {
		nodo_actual = hash->vector[posicion];
		while (continuar && nodo_actual != NULL) {
			continuar =
				f(nodo_actual->clave, nodo_actual->valor, aux);
			nodo_actual = nodo_actual->siguiente;
			recorridos++;
		}
		posicion++;
	}
	return recorridos;
}
