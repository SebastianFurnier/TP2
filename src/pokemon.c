#include "pokemon.h"
#include <string.h>
#include <stdio.h>
#include "pokemon_privado.h"

#define CANTIDAD_COMAS 3
#define ERROR 0

pokemon_t *pokemon_crear_desde_string(const char *string)
{
	if ((string == NULL) || (strcmp(string, "") == 0))
		return NULL;

	int comas_leidas = 0;
	size_t largo_cadena = strlen(string);
	int i = 0;

	for (i = 0; i < largo_cadena; i++) {
		if (string[i] == ',')
			comas_leidas++;
	}

	if (comas_leidas != CANTIDAD_COMAS)
		return NULL;

	pokemon_t *pokemon_creado = calloc(1, sizeof(pokemon_t));

	if (pokemon_creado == NULL)
		return NULL;

	sscanf(string, "%zu,%[^,],%zu,%[^,]", &pokemon_creado->id,
	       pokemon_creado->nombre, &pokemon_creado->salud,
	       pokemon_creado->nombre_entrenador);

	return pokemon_creado;
}

pokemon_t *pokemon_copiar(pokemon_t *poke)
{
	if (poke == NULL) {
		return NULL;
	}
	pokemon_t *copia = calloc(1, sizeof(pokemon_t));
	if (copia == NULL)
		return NULL;

	copia->id = poke->id;
	copia->salud = poke->salud;
	strcpy(copia->nombre, poke->nombre);
	strcpy(copia->nombre_entrenador, poke->nombre_entrenador);

	return copia;
}

bool pokemon_son_iguales(pokemon_t *pokemon1, pokemon_t *pokemon2)
{
	if (pokemon1 == NULL || pokemon2 == NULL)
		return false;
	if ((pokemon1->id == pokemon2->id) &&
	    (pokemon1->salud == pokemon2->salud) &&
	    (strcmp(pokemon1->nombre, pokemon2->nombre) == 0) &&
	    (strcmp(pokemon1->nombre_entrenador, pokemon2->nombre_entrenador) ==
	     0))
		return true;
	return false;
}

char *pokemon_nombre(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return NULL;
	return pokemon->nombre;
}

char *pokemon_entrenador(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return NULL;
	return pokemon->nombre_entrenador;
}

size_t pokemon_salud(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return ERROR;
	return pokemon->salud;
}

size_t pokemon_id(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return ERROR;
	return pokemon->id;
}

void pokemon_destruir(pokemon_t *pokemon)
{
	free(pokemon);
	return;
}
