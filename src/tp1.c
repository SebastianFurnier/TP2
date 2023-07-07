#include "tp1.h"
#include "pokemon.h"
#include "abb.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_PUNTEROS 0
#define MAX_LETRAS_DATOS_POKEMONES 30

struct _hospital_pkm_t {
	abb_t* pokemones;
	size_t cantidad_pokemon;
	size_t cantidad_entrenadores;
};

int comparador_aux(void *pokemon_uno, void *pokemon_dos)
{
	pokemon_t* poke_aux_uno = (pokemon_t*)pokemon_uno;
	pokemon_t* poke_aux_dos = (pokemon_t*)pokemon_dos;

	if (pokemon_salud(poke_aux_uno) > pokemon_salud(poke_aux_dos))
		return 1;
	return -1;
}

void limpiar_cadena(char cadena_a_limpiar[MAX_LETRAS_DATOS_POKEMONES])
{
	int i;

	for(i = 0; i<MAX_LETRAS_DATOS_POKEMONES; i++)
		cadena_a_limpiar[i] = 0;
}

void cerrar_destruir(hospital_t *hospital, FILE *archivo, pokemon_t *pokemon)
{
	if (hospital->pokemones != NULL)
		abb_destruir_todo(hospital->pokemones, free);
	free(hospital);
	pokemon_destruir(pokemon);
	if (archivo != NULL)
		fclose(archivo);

	return;
}

hospital_t *hospital_crear_desde_archivo(const char *nombre_archivo)
{
	if ((!nombre_archivo) ||
	    (strstr(nombre_archivo, ".txt") == false))
		return NULL;

	FILE *archivo = fopen(nombre_archivo, "r");

	if (!archivo)
		return NULL;

	hospital_t *hospital = malloc(sizeof(hospital_t));

	if (!hospital) {
		fclose(archivo);
		return NULL;
	}

	hospital->pokemones = abb_crear(comparador_aux);
	hospital->cantidad_pokemon = 0;
	hospital->cantidad_entrenadores = 0;

	if (!hospital->pokemones){
		cerrar_destruir(hospital, archivo, NULL);
		return NULL;
	}
	pokemon_t* nuevo_pokemon = NULL;
	char datos_pokemon_archivo[MAX_LETRAS_DATOS_POKEMONES];

	while (fgets(datos_pokemon_archivo, MAX_LETRAS_DATOS_POKEMONES, archivo)){

		datos_pokemon_archivo[strcspn(datos_pokemon_archivo, "\n")] = '\0';
		nuevo_pokemon = pokemon_crear_desde_string(datos_pokemon_archivo);

		if (!nuevo_pokemon){
			cerrar_destruir(hospital, archivo, NULL);
			return NULL;
		}
		if (!abb_insertar(hospital->pokemones, nuevo_pokemon)){
			cerrar_destruir(hospital, archivo, nuevo_pokemon);
			return NULL;
		}

		hospital->cantidad_pokemon++;
	}

	fclose(archivo);

	return hospital;
}

size_t hospital_cantidad_pokemones(hospital_t *hospital)
{
	if (hospital == NULL)
		return ERROR_PUNTEROS;
	return hospital->cantidad_pokemon;
}

bool funcio_para_abb(void* pokemon, void* args)
{
	if (!args || !pokemon)
		return false;

    bool (*funcion)(pokemon_t *p, void *aux) = *(bool (**)(pokemon_t *, void *))(*(void **)args);
    
	void* aux_ptr = *((void**)args + 1);

	return funcion(pokemon, aux_ptr);
}


size_t hospital_a_cada_pokemon(hospital_t *hospital,
			       bool (*funcion)(pokemon_t *p, void *aux),
			       void *aux)
{
	if (!funcion || !hospital)
		return ERROR_PUNTEROS;//cambiar nombre

	void* args_funcion[2];
	args_funcion[0] = &funcion;
	args_funcion[1] =  aux;
	return abb_con_cada_elemento(hospital->pokemones, INORDEN, funcio_para_abb, args_funcion);
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

bool hospital_obtener_abb(void* uno, void* arg)
{
    pokemon_t* pokemon_aux = (pokemon_t*)uno;
    if (!pokemon_aux)
        return false;

    size_t* prioridad = (size_t*)(((void**)arg)[1]);
    size_t* contador = (size_t*)(((void**)arg)[2]);
    pokemon_t** pokemon_buscado = (pokemon_t**)(((void**)arg)[0]);

    if (*contador == *prioridad) {
        *pokemon_buscado = pokemon_aux;
        return false;
    }
    
    (*contador)++;
    return true;
}

pokemon_t* hospital_obtener_pokemon(hospital_t* hospital, size_t prioridad)
{
    if (hospital == NULL || abb_tamanio(hospital->pokemones) < prioridad)
        return NULL;

    pokemon_t* pokemon_buscado = NULL;
    size_t contador = 0;
    void* arg_funcion[3];
    arg_funcion[0] = &pokemon_buscado;
    arg_funcion[1] = &prioridad;
    arg_funcion[2] = &contador;
    abb_con_cada_elemento(hospital->pokemones, INORDEN, hospital_obtener_abb, arg_funcion);

    return pokemon_buscado;
}

void hospital_destruir(hospital_t *hospital)
{
	if (!hospital)
		return;
	cerrar_destruir(hospital, NULL, NULL);
	return;
}
