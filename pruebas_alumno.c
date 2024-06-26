#include "pa2m.h"
#include "./menu/menu.h"

bool funcion_prueba(void *contexto, void *contexto_dos)
{
	return true;
}

void pruebas_quitar_opcion()
{
	menu_t *nuevo_menu = crear_menu();

	crear_opcion(nuevo_menu, "A", "titulo_a", "descripcion_a",
		     funcion_prueba);
	crear_opcion(nuevo_menu, "B", "titulo_b", "descripcion_b",
		     funcion_prueba);
	crear_opcion(nuevo_menu, "C", "titulo_b", "descripcion_c",
		     funcion_prueba);

	bool correcto = menu_quitar_opcion(NULL, 0);
	pa2m_afirmar(
		!correcto,
		"Intentar eliminar una opcion de un menu invalido devuelve false.");

	correcto = true;

	correcto = menu_quitar_opcion(nuevo_menu, 10);
	pa2m_afirmar(
		!correcto,
		"Intentar eliminar una opcion con un ID invalido devuelve false.");

	correcto = false;

	correcto = menu_quitar_opcion(nuevo_menu, 0);
	pa2m_afirmar(
		correcto,
		"Eliminar una opcion valida de un menu valido devuelve true.");

	pa2m_afirmar(
		menu_cantidad(nuevo_menu) == 2,
		"Luego de eliminar una opcion la cantidad de opciones es la correcta.");

	menu_destruir(nuevo_menu);
}

void pruebas_mostrar_opciones()
{
	menu_t *nuevo_menu = crear_menu();

	crear_opcion(nuevo_menu, "A", "titulo_a", "descripcion_a",
		     funcion_prueba);
	crear_opcion(nuevo_menu, "B", "titulo_b", "descripcion_b",
		     funcion_prueba);
	crear_opcion(nuevo_menu, "C", "titulo_b", "descripcion_c",
		     funcion_prueba);

	bool correcto = menu_mostrar_opciones(NULL, false);
	pa2m_afirmar(
		!correcto,
		"Se intenta mostrar las opciones de un menu invalido y devuelve false.");

	correcto = false;

	correcto = menu_mostrar_opciones(nuevo_menu, false);
	pa2m_afirmar(correcto,
		     "Se muestran todas las opciones de un menu valido.");

	menu_destruir(nuevo_menu);
}

void pruebas_seleccionar_opcion()
{
	menu_t *nuevo_menu = crear_menu();

	crear_opcion(nuevo_menu, "A", "titulo_a", "descripcion_a",
		     funcion_prueba);
	crear_opcion(nuevo_menu, "B", "titulo_b", "descripcion_b",
		     funcion_prueba);
	crear_opcion(nuevo_menu, "C", "titulo_b", "descripcion_c",
		     funcion_prueba);

	bool correcto = menu_seleccionar_opcion(nuevo_menu, "", NULL);
	pa2m_afirmar(
		!correcto,
		"Se intenta seleecionar una opcion con titulo vacio y devuelve false.");

	correcto = true;

	correcto = menu_seleccionar_opcion(nuevo_menu, "D", NULL);
	pa2m_afirmar(
		!correcto,
		"Se intenta seleccionar una opcion con un titulo invalido y devuelve false.");

	correcto = false;

	correcto = menu_seleccionar_opcion(nuevo_menu, "A", NULL);
	pa2m_afirmar(
		correcto,
		"Se selecciona una opcion con un titulo valido y devuelve true.");

	menu_destruir(nuevo_menu);
}

void pruebas_menu_opcion()
{
	menu_t *nuevo_menu = crear_menu();

	pa2m_afirmar(nuevo_menu != NULL, "Se crea un menu correctamente.");
	pa2m_afirmar(menu_cantidad(nuevo_menu) == 0,
		     "Un nuevo menu se crea con cantidad cero.");

	bool correcto =
		crear_opcion(nuevo_menu, "opcion", "p", "descripcion", NULL);
	pa2m_afirmar(
		!correcto,
		"Se intenta crear una opcion con funcion NULL y devuelve false.");

	//Me aseguro que correcto tenga siempre el valor contrario que espero devuelva la funcion a evaluar.
	correcto = true;

	correcto = crear_opcion(NULL, "opcion", "p", "descripcion",
				funcion_prueba);
	pa2m_afirmar(
		!correcto,
		"Se intenta crear una opcion con un menu invalido y devuelve false.");

	correcto = true;

	correcto =
		crear_opcion(nuevo_menu, "", "", "descripcion", funcion_prueba);
	pa2m_afirmar(
		!correcto,
		"Se intenta crear una opcion sin titulos y devuelve false");

	correcto = false;

	correcto = crear_opcion(nuevo_menu, "titulo", "titulo_dos",
				"descripcion", funcion_prueba);
	pa2m_afirmar(correcto,
		     "Se ingresan datos correctos y se crea la opcion.");

	pa2m_afirmar(
		menu_cantidad(nuevo_menu) == 1,
		"Despues de las pruebas la cantidad de opciones en el menu es la correcta.");

	menu_destruir(nuevo_menu);
}

int main()
{
	pa2m_nuevo_grupo("Pruebas de creacion de menu.");
	pruebas_menu_opcion();
	pa2m_nuevo_grupo("Pruebas de seleccion de opcion.");
	pruebas_seleccionar_opcion();
	pa2m_nuevo_grupo("Pruebas mostrar opciones.");
	pruebas_mostrar_opciones();
	pa2m_nuevo_grupo("Pruebas de eliminacion de opciones.");
	pruebas_quitar_opcion();

	return pa2m_mostrar_reporte();
}