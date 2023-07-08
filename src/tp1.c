#include "tp1.h"
#include "pokemon.h"
#include "abb.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define VACIO 0
#define MAX_DATOS_POKEMONES 30

struct _hospital_pkm_t {
	abb_t *pokemones;
	size_t cantidad_pokemon;
	size_t cantidad_entrenadores;
};

/**
 * Funcion de comparacion que se usa para crear el ABB donde alojo los pokemones.
*/
int comparador_aux(void *pokemon_uno, void *pokemon_dos)
{
	pokemon_t *poke_aux_uno = (pokemon_t *)pokemon_uno;
	pokemon_t *poke_aux_dos = (pokemon_t *)pokemon_dos;

	if (pokemon_salud(poke_aux_uno) > pokemon_salud(poke_aux_dos))
		return 1;
	return -1;
}

void cerrar_destruir(hospital_t *hospital, FILE *archivo, pokemon_t *pokemon)
{
	abb_destruir_todo(hospital->pokemones, free);
	free(hospital);
	pokemon_destruir(pokemon);
	if (archivo != NULL)
		fclose(archivo);

	return;
}

bool manejar_datos_archivo(hospital_t *hospital, FILE *archivo)
{
	pokemon_t *nuevo_pokemon = NULL;
	char datos_pokemon_archivo[MAX_DATOS_POKEMONES];

	while (fgets(datos_pokemon_archivo, MAX_DATOS_POKEMONES, archivo)) {
		datos_pokemon_archivo[strcspn(datos_pokemon_archivo, "\n")] =
			'\0';
		nuevo_pokemon =
			pokemon_crear_desde_string(datos_pokemon_archivo);

		if (!nuevo_pokemon ||
		    !abb_insertar(hospital->pokemones, nuevo_pokemon)) {
			cerrar_destruir(hospital, archivo, nuevo_pokemon);
			return false;
		}

		hospital->cantidad_pokemon++;
	}

	return true;
}

hospital_t *crear_iniciar_hospital(FILE *archivo)
{
	hospital_t *hospital = malloc(sizeof(hospital_t));

	if (!hospital) {
		cerrar_destruir(NULL, archivo, NULL);
		return NULL;
	}

	hospital->pokemones = abb_crear(comparador_aux);
	hospital->cantidad_pokemon = 0;
	hospital->cantidad_entrenadores = 0;

	if (!hospital->pokemones) {
		cerrar_destruir(hospital, archivo, NULL);
		return NULL;
	}

	bool carga_exitosa = manejar_datos_archivo(hospital, archivo);

	if (!carga_exitosa)
		return NULL;

	fclose(archivo);

	return hospital;
}

hospital_t *hospital_crear_desde_archivo(const char *nombre_archivo)
{
	if ((!nombre_archivo) || !strstr(nombre_archivo, ".txt"))
		return NULL;

	FILE *archivo = fopen(nombre_archivo, "r");

	if (!archivo)
		return NULL;

	return crear_iniciar_hospital(archivo);
}

size_t hospital_cantidad_pokemones(hospital_t *hospital)
{
	if (!hospital)
		return VACIO;
	return hospital->cantidad_pokemon;
}

bool funcion_para_abb(void *pokemon, void *args)
{
	if (!args || !pokemon)
		return false;

	bool (*funcion)(pokemon_t * p, void *aux) =
		*(bool (**)(pokemon_t *, void *))(*(void **)args);

	void *aux_ptr = *((void **)args + 1);
	return funcion(pokemon, aux_ptr);
}

size_t hospital_a_cada_pokemon(hospital_t *hospital,
			       bool (*funcion)(pokemon_t *p, void *aux),
			       void *aux)
{
	if (!funcion || !hospital)
		return VACIO;

	void *args_funcion[2];

	args_funcion[0] = &funcion;
	args_funcion[1] = aux;

	return abb_con_cada_elemento(hospital->pokemones, INORDEN,
				     funcion_para_abb, args_funcion);
}

int hospital_aceptar_emergencias(hospital_t *hospital,
				 pokemon_t **pokemones_ambulancia,
				 size_t cant_pokes_ambulancia)
{
	if (!hospital || !pokemones_ambulancia)
		return ERROR;
	if (cant_pokes_ambulancia == 0)
		return EXITO;

	int i;

	for (i = 0; i < cant_pokes_ambulancia; i++) {
		abb_insertar(hospital->pokemones, pokemones_ambulancia[i]);
	}

	hospital->cantidad_pokemon =
		hospital_cantidad_pokemones(hospital) + cant_pokes_ambulancia;
	return (EXITO);
}

bool hospital_obtener_abb(pokemon_t *pokemon_abb, void *arg)
{
	if (!pokemon_abb)
		return false;

	pokemon_t **pokemon_buscado = (pokemon_t **)(((void **)arg)[0]);
	size_t *prioridad = (size_t *)(((void **)arg)[1]);
	size_t *contador = (size_t *)(((void **)arg)[2]);

	if (*contador == *prioridad) {
		*pokemon_buscado = pokemon_abb;
		(*contador)++;
		return false;
	}

	(*contador)++;

	return true;
}

pokemon_t *hospital_obtener_pokemon(hospital_t *hospital, size_t prioridad)
{
	if (!hospital || abb_tamanio(hospital->pokemones) < prioridad)
		return NULL;

	pokemon_t *pokemon_buscado = NULL;
	size_t contador = 0;
	void *arg_funcion[3];

	arg_funcion[0] = &pokemon_buscado;
	arg_funcion[1] = &prioridad;
	arg_funcion[2] = &contador;

	hospital_a_cada_pokemon(hospital, hospital_obtener_abb, arg_funcion);

	return pokemon_buscado;
}

void hospital_destruir(hospital_t *hospital)
{
	if (!hospital)
		return;
	cerrar_destruir(hospital, NULL, NULL);
	return;
}
