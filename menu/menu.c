#include "../src/tp1.h"
#include "./lista.h"

#include "string.h"
#include "stdio.h"
#include "ctype.h"
#include "stdbool.h"

#define CANT_MAX_TITULO 10
#define CANT_MAX_DESCRIPCION 100

typedef struct
{
	char titulo_uno[CANT_MAX_TITULO];
	char titulo_dos[CANT_MAX_TITULO];
	bool (*funcion)(void*, void*);
	char descripcion[CANT_MAX_DESCRIPCION];
}opcion_t;

typedef struct menu
{
	lista_t* opciones;
	size_t cantidad_opciones;
}menu_t;

int comparador(void* elemento_uno, void* elemento_dos)
{
	opcion_t* aux_uno = (opcion_t*)elemento_uno;
	char* titulo_aux = (char*)elemento_dos;

	int comparacion_uno = strcmp(aux_uno->titulo_uno, titulo_aux);
	int comparacion_dos = strcmp(aux_uno->titulo_dos, titulo_aux);

	if (comparacion_uno == 0 || comparacion_dos == 0)
		return 0;
	return 1;
}

void texto_a_minuscula(char texto[])
{
    int i = 0;
    while (texto[i] != '\0')
    {
        texto[i] = (char)tolower((unsigned char)texto[i]);
        i++;
    }
}

opcion_t* crear_opcion(char* titulo_uno, char* titulo_dos, char* descripcion, bool (*f)(void*, void*))
{
	if ((strlen(titulo_uno) == 0 && strlen(titulo_dos) == 0)){
		printf("No puede crearse una opcion sin titulo");
		return NULL;
	}

//	if (!f)
//		return NULL;

	opcion_t* nueva_opcion = malloc(sizeof(opcion_t));

	if (!nueva_opcion)
		return NULL;

	strcpy(nueva_opcion->titulo_uno, titulo_uno);
	strcpy(nueva_opcion->titulo_dos, titulo_dos);
	strcpy(nueva_opcion->descripcion, descripcion);
	nueva_opcion->funcion = f;

	texto_a_minuscula(nueva_opcion->titulo_uno);
	texto_a_minuscula(nueva_opcion->titulo_dos);
	texto_a_minuscula(nueva_opcion->descripcion);

	return nueva_opcion;
}

void menu_mostrar_opciones(menu_t* menu)
{
	lista_iterador_t* iterador_opciones = lista_iterador_crear(menu->opciones);
	if (!iterador_opciones)
		return;
	
	opcion_t* opcion_actual = (opcion_t*)lista_iterador_elemento_actual(iterador_opciones);

	printf("\nSeleccione una de las opciones:\n\n");

	while (opcion_actual)
	{
		printf("- %s (%s).\n", opcion_actual->titulo_uno, opcion_actual->titulo_dos);
		lista_iterador_avanzar(iterador_opciones);
		opcion_actual = lista_iterador_elemento_actual(iterador_opciones);
	}
	lista_iterador_destruir(iterador_opciones);
}

bool mostrar_descripcion(void* opcion, void* contexto)
{
	if (!opcion)
		return false;
	opcion_t* opcion_actual = (opcion_t*)opcion;
	printf("\n- %s (%s): %s.\n",opcion_actual->titulo_uno, opcion_actual->titulo_dos, opcion_actual->descripcion);
	return true;
}

bool menu_mostrar_ayuda(void* menu, void* contexto)
{
	menu_t* menu_aux = (menu_t*)menu;
	size_t cantidad_recorrida = lista_con_cada_elemento(menu_aux->opciones, mostrar_descripcion, NULL);

	if(cantidad_recorrida != menu_aux->cantidad_opciones)
		return false;
	return true;
}

bool menu_seleccionar_opcion(menu_t* menu, char opcion[], void* contexto, void* contexto_aux)
{
	size_t largo = strlen(opcion);
	char titulo_aux[largo];
	strcpy(titulo_aux, opcion);
	titulo_aux[largo-1] = '\0';
	texto_a_minuscula(titulo_aux);

	opcion_t* opcion_actual = lista_buscar_elemento(menu->opciones, comparador, titulo_aux);
	if (!opcion_actual){
		printf("\nIngrese una opcion valida.");
		return true;
	}

	if(opcion_actual->funcion(contexto, contexto_aux))
		return true;
	return false;
}

menu_t* crear_menu()
{
	menu_t* nuevo_menu = malloc(sizeof(menu_t));
	lista_t* lista_opciones = lista_crear();

	if (!nuevo_menu || !lista_opciones)
		return NULL;
	
	nuevo_menu->opciones = lista_opciones;
	nuevo_menu->cantidad_opciones = 0;
	
	return nuevo_menu;
}

menu_t* insertar_opcion_menu(menu_t* menu, opcion_t* nueva_opcion)
{
	if (!menu || !nueva_opcion)
		return NULL;
	void* puntero_valido = lista_insertar(menu->opciones, nueva_opcion);
	if (!puntero_valido)
		return NULL;
	menu->cantidad_opciones++;
	return menu;
}

void destruir_menu(menu_t* menu)
{
	lista_destruir_todo(menu->opciones, free);
	free(menu);
}
