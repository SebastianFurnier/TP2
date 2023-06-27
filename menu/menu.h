#ifndef __MENU_H__
#define __MENU_H__

#include <stdbool.h>
#include <stddef.h>

#include "../src/tp1.h"
#include "./lista.h"

typedef struct opcion opcion_t;
typedef struct menu menu_t;

/*
 * Crea un menu reservando la memoria necesaria, devolviendo un puntero al TDA creado o
 * NULL en caso de error.
 */
menu_t *crear_menu();

/*
 * Recibe un menu, un string y un puntero auxiliar.
 * Busca el string ingresado y si encuentra la opcion correspondiente invoca
 * la funcion alojada en ella y devuelve true.
 * En caso de no encontrar la opcion o fallar la funcion devuelve false.
 */
bool menu_seleccionar_opcion(menu_t *menu, char opcion[], void *contexto_aux);

/*
 * Recibe un menu y (si mostrar es true) muestra todas las opciones alojadas en el y devuelve true, 
 * en caso de fallar, no poder mostrar todas las opciones, devuelve false.
 */
bool menu_mostrar_opciones(menu_t *menu, bool mostrar);

/*
 * Muestra la descripcion alojada en cada TDA opcion.
 */
bool menu_mostrar_descripcion(void *menu, void *contexto);

/*
 * Devuelve la cantidad de opciones ingresada en el menu.
 */

size_t menu_cantidad(menu_t *menu);

/**
 * Recibe un menu y un id, si estos son validos elimina la opcion del menu. En caso de poder eliminarla
 * devuelve true, caso contrario false.
*/
bool menu_quitar_opcion(menu_t *menu, size_t id);

/*
 * Crea un TDA opcion a partir de los datos pasados por parametro y lo inserta en el menu.
 * En caso de que se hayan pasado 2 titulos vacios o no se haya ingresado una
 * funcion, devuelve false.
 * En caso de exito devuelve true.
 */
bool crear_opcion(menu_t *menu, char *titulo_uno, char *titulo_dos,
		  char *descripcion, bool (*f)(void *, void *));

/*
 * Destruye el menu y todas las opciones cargadas.
 */
void menu_destruir(menu_t *menu);

#endif /* __MENU_H__ */
