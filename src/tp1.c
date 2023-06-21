#include "tp1.h"
#include "pokemon.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_PUNTEROS 0

struct _hospital_pkm_t {
	pokemon_t **pokemones;
	size_t cantidad_pokemon;
	size_t cantidad_entrenadores;
	char nombre[100];
};

void cerrar_destruir(hospital_t *hospital, FILE *archivo, pokemon_t *pokemon,
		     char *aux)
{
	hospital_destruir(hospital);
	pokemon_destruir(pokemon);
	fclose(archivo);
	free(aux);

	return;
}

void ordenar_por_salud(hospital_t *hospital, size_t cantidad_pokemones)
{
	size_t i, j;
	pokemon_t *pokm_aux;

	for (i = 1; i < cantidad_pokemones; i++) {
		for (j = 0; j < cantidad_pokemones - i; j++) {
			if (pokemon_salud(hospital->pokemones[j]) >
			    pokemon_salud(hospital->pokemones[j + 1])) {
				pokm_aux = hospital->pokemones[j];
				hospital->pokemones[j] =
					hospital->pokemones[j + 1];
				hospital->pokemones[j + 1] = pokm_aux;
			}
		}
	}
	return;
}

hospital_t *hospital_crear_desde_archivo(const char *nombre_archivo)
{
	if ((nombre_archivo == NULL) ||
	    (strstr(nombre_archivo, ".txt") == false))
		return NULL;

	FILE *archivo = fopen(nombre_archivo, "r");

	if (archivo == NULL)
		return NULL;

	hospital_t *hospital = calloc(1, sizeof(hospital_t));

	if (hospital == NULL) {
		fclose(archivo);
		return NULL;
	}

	hospital->pokemones = calloc(1, sizeof(pokemon_t **));
	if (hospital->pokemones == NULL) {
		cerrar_destruir(hospital, archivo, NULL, NULL);
		return NULL;
	}

	size_t leido = 0;
	size_t cantidad_pokemones = 0;
	pokemon_t **pkm_aux = NULL;
	pokemon_t *aux_desde_string = NULL;
	char caracter;
	char *aux = NULL;
	char *aux_str;
	aux_str = calloc(1, sizeof(char));

	if (aux_str == NULL) {
		cerrar_destruir(hospital, archivo, NULL, NULL);
		return NULL;
	}

	caracter = (char)fgetc(archivo);

	while ((caracter != EOF) && (caracter != ' ')) {
		while ((caracter != '\n')) {
			aux_str[leido] = caracter;
			leido++;
			aux = realloc(aux_str, (leido + 1) * sizeof(char));

			if (aux == NULL) {
				cerrar_destruir(hospital, archivo,
						aux_desde_string, NULL);
				return NULL;
			}
			aux_str = aux;
			aux_str[leido] = 0;
			caracter = (char)fgetc(archivo);
		}

		pkm_aux = realloc(hospital->pokemones,
				  (cantidad_pokemones + 1) *
					  (sizeof(pokemon_t *)));

		if (pkm_aux == NULL) {
			cerrar_destruir(hospital, archivo, aux_desde_string,
					NULL);
			return NULL;
		}

		hospital->pokemones = pkm_aux;

		aux_desde_string = pokemon_crear_desde_string(aux_str);

		if (aux_desde_string == NULL) {
			cerrar_destruir(hospital, archivo, aux_desde_string,
					aux);
			return NULL;
		}

		hospital->pokemones[cantidad_pokemones] = aux_desde_string;

		aux_str = realloc(aux_str, sizeof(char));
		if (aux_str == NULL) {
			cerrar_destruir(hospital, archivo, NULL, aux_str);
			return NULL;
		}

		leido = 0;

		cantidad_pokemones++;

		caracter = (char)fgetc(archivo);
	}

	hospital->cantidad_pokemon = cantidad_pokemones;
	strcpy(hospital->nombre, nombre_archivo);

	ordenar_por_salud(hospital, cantidad_pokemones);

	cerrar_destruir(NULL, archivo, NULL, aux_str);

	return hospital;
}

char* hospital_nombre(hospital_t* hospital)
{
	if (!hospital)
		return "";
	return hospital->nombre;
}

size_t hospital_cantidad_pokemones(hospital_t *hospital)
{
	if (hospital == NULL)
		return ERROR_PUNTEROS;
	return hospital->cantidad_pokemon;
}

size_t hospital_a_cada_pokemon(hospital_t *hospital,
			       bool (*funcion)(pokemon_t *p, void *aux),
			       void *aux)
{
	if ((funcion == NULL) || (hospital == NULL))
		return ERROR_PUNTEROS;

	size_t i = 0;
	bool seguir = true;
	while (seguir && (i < hospital->cantidad_pokemon)) {
		seguir = funcion((hospital->pokemones[i]), aux);
		i++;
	}
	return i;
}

int hospital_aceptar_emergencias(hospital_t *hospital,
				 pokemon_t **pokemones_ambulancia,
				 size_t cant_pokes_ambulancia)
{
	if ((hospital == NULL) || (pokemones_ambulancia == NULL))
		return ERROR;
	if (cant_pokes_ambulancia == 0)
		return EXITO;

	size_t cantidad_pokemones =
		hospital_cantidad_pokemones(hospital) + cant_pokes_ambulancia;
	pokemon_t **aux = realloc(hospital->pokemones,
				  cantidad_pokemones * sizeof(pokemon_t *));

	if (aux == NULL) {
		free(hospital);
		return ERROR;
	}
	hospital->pokemones = aux;

	size_t i;
	size_t j = hospital_cantidad_pokemones(hospital);

	for (i = 0; i < cant_pokes_ambulancia; i++) {
		hospital->pokemones[j] = pokemones_ambulancia[i];
		j++;
	}

	ordenar_por_salud(hospital, cantidad_pokemones);

	hospital->cantidad_pokemon =
		hospital_cantidad_pokemones(hospital) + cant_pokes_ambulancia;
	return (EXITO);
}

pokemon_t *hospital_obtener_pokemon(hospital_t *hospital, size_t prioridad)
{
	if ((hospital == NULL) || (hospital->pokemones[prioridad] == NULL))
		return NULL;

	return hospital->pokemones[prioridad];
}

void hospital_destruir(hospital_t *hospital)
{
	if (hospital == NULL)
		return;
	int i = 0;
	for (i = 0; i < hospital->cantidad_pokemon; i++)
		free(hospital->pokemones[i]);
	free(hospital->pokemones);
	free(hospital);
	return;
}
