#include "../src/tp1.h"
#include "./lista.h"
#include "menu.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define LETRAS_MAX_TITULO 10
#define LETRAS_MAX_DESCRIPCION 100
#define ERROR -1
#define EXITO 0

struct opcion {
	char titulo_uno[LETRAS_MAX_TITULO];
	char titulo_dos[LETRAS_MAX_TITULO];
	char descripcion[LETRAS_MAX_DESCRIPCION];
	bool (*funcion)(void *, void *);
};

struct menu {
	lista_t *opciones;
	size_t cantidad_opciones;
};

/*
 * Funcion privada del TDA menu. Recibe dos parametros, el cual el primero es un TDA opcion y el segundo un string.
 * Compara dicho string con los titulo almacenados en el TDA opcion, en caso de no encontrarse devuelve error.
 */
int comparador(void *elemento_uno, void *titulo_a_buscar)
{
	opcion_t *opcion = (opcion_t *)elemento_uno;
	char *titulo_aux = (char *)titulo_a_buscar;

	if (!opcion || strlen(titulo_aux) == 0)
		return ERROR;

	int comparacion_uno = strcmp(opcion->titulo_uno, titulo_aux);
	int comparacion_dos = strcmp(opcion->titulo_dos, titulo_aux);

	if (comparacion_uno == 0 || comparacion_dos == 0)
		return EXITO;

	return ERROR;
}

/*
 * Funcion privada del TDA menu, recibe un string y lo convierte a minusculas.
 */
void texto_a_minuscula(char texto[])
{
	int i = 0;
	while (texto[i] != '\0') {
		texto[i] = (char)tolower((unsigned char)texto[i]);
		i++;
	}
}

menu_t *crear_menu()
{
	menu_t *nuevo_menu = malloc(sizeof(menu_t));

	if (!nuevo_menu)
		return NULL;

	lista_t *lista_opciones = lista_crear();

	if (!lista_opciones) {
		free(nuevo_menu);
		return NULL;
	}

	nuevo_menu->opciones = lista_opciones;
	nuevo_menu->cantidad_opciones = 0;

	return nuevo_menu;
}

bool menu_seleccionar_opcion(menu_t *menu, char titulo_opcion_buscado[],
			     void *contexto_aux)
{
	if (!menu || strlen(titulo_opcion_buscado) == 0)
		return false;

	size_t largo_titulo = strlen(titulo_opcion_buscado);
	char titulo_aux[largo_titulo];

	strcpy(titulo_aux, titulo_opcion_buscado);

	if (titulo_aux[largo_titulo - 1] == '\n')
		titulo_aux[largo_titulo - 1] = '\0';

	texto_a_minuscula(titulo_aux);

	opcion_t *opcion_actual =
		lista_buscar_elemento(menu->opciones, comparador, titulo_aux);

	if (!opcion_actual)
		return true;

	if (opcion_actual->funcion(menu, contexto_aux))
		return true;

	return false;
}

bool menu_mostrar_opciones(menu_t *menu)
{
	if (!menu)
		return false;

	lista_iterador_t *iterador_opciones =
		lista_iterador_crear(menu->opciones);

	if (!iterador_opciones)
		return false;

	opcion_t *opcion_actual =
		(opcion_t *)lista_iterador_elemento_actual(iterador_opciones);

	int cantidad_opciones_mostradas = 0;

	printf("\nSeleccione una de las opciones:\n\n");

	while (opcion_actual) {
		cantidad_opciones_mostradas++;

		printf("- %s (%s).\n", opcion_actual->titulo_uno,
		       opcion_actual->titulo_dos);

		lista_iterador_avanzar(iterador_opciones);

		opcion_actual =
			lista_iterador_elemento_actual(iterador_opciones);
	}

	lista_iterador_destruir(iterador_opciones);

	if (cantidad_opciones_mostradas == menu->cantidad_opciones)
		return true;

	return false;
}

/*
 * Funcion privada del TDA, auxiliar de menu_mostrar_descripcion. Imprime por pantalla la descripcion alojada en cada
 * TDA opcion.
 */
bool mostrar_descripcion_aux(void *opcion, void *contexto)
{
	opcion_t *opcion_actual = (opcion_t *)opcion;

	if (!opcion_actual)
		return false;

	printf("\n- %s (%s): %s.\n", opcion_actual->titulo_uno,
	       opcion_actual->titulo_dos, opcion_actual->descripcion);
	return true;
}

bool menu_mostrar_descripcion(void *menu, void *contexto)
{
	menu_t *menu_aux = (menu_t *)menu;

	size_t cantidad_recorrida = lista_con_cada_elemento(
		menu_aux->opciones, mostrar_descripcion_aux, NULL);

	if (cantidad_recorrida != menu_aux->cantidad_opciones)
		return false;

	return true;
}

size_t menu_cantidad(menu_t *menu)
{
	if (!menu)
		return 0;
	return menu->cantidad_opciones;
}

/*
 * Funcion privada del TDA. Inserta la opcion en el menu pasado a la funcion.
 */
bool insertar_opcion_menu(menu_t *menu, opcion_t *nueva_opcion)
{
	if (!menu || !nueva_opcion)
		return false;

	lista_t *puntero_lista_aux =
		lista_insertar(menu->opciones, nueva_opcion);

	if (!puntero_lista_aux)
		return false;

	menu->cantidad_opciones++;

	return true;
}

bool crear_opcion(menu_t *menu, char *titulo_uno, char *titulo_dos,
		  char *descripcion, bool (*f)(void *, void *))
{
	if ((strlen(titulo_uno) == 0 && strlen(titulo_dos) == 0) || !f || !menu)
		return false;

	opcion_t *nueva_opcion = malloc(sizeof(opcion_t));

	if (!nueva_opcion)
		return false;

	strcpy(nueva_opcion->titulo_uno, titulo_uno);
	strcpy(nueva_opcion->titulo_dos, titulo_dos);
	strcpy(nueva_opcion->descripcion, descripcion);
	nueva_opcion->funcion = f;

	texto_a_minuscula(nueva_opcion->titulo_uno);
	texto_a_minuscula(nueva_opcion->titulo_dos);
	texto_a_minuscula(nueva_opcion->descripcion);

	return insertar_opcion_menu(menu, nueva_opcion);
}

void menu_destruir(menu_t *menu)
{
	lista_destruir_todo(menu->opciones, free);
	free(menu);
}
