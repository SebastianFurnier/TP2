#include "./menu/menu.c"

#define MAX_STRING 10

int main()
{
	menu_t* menu = crear_menu();

	char* titulo_uno_salir = "S";
	char* titulo_dos_salir = "salir";

	char* titulo_uno_ayuda = "H";
	char* titulo_dos_ayuda = "ayuda";

	char* titulo_uno_cargar= "C";
	char* titulo_dos_cargar = "cargar";

	char* titulo_uno_estado = "E";
	char* titulo_dos_estado = "estado";

	char* titulo_uno_activar = "A";
	char* titulo_dos_activar = "Activar";

	char* titulo_uno_mostrar = "M";
	char* titulo_dos_mostrar = "mostrar";

	char* titulo_uno_listar = "L";
	char* titulo_dos_listar = "listar";

	char* titulo_uno_destruir = "D";
	char* titulo_dos_destruir = "destruir";

	opcion_t* opcion_salir = crear_opcion(titulo_uno_salir, titulo_dos_salir, "", NULL);
	opcion_t* opcion_ayuda = crear_opcion(titulo_uno_ayuda, titulo_dos_ayuda, "", NULL);
	opcion_t* opcion_cargar = crear_opcion(titulo_uno_cargar, titulo_dos_cargar, "", NULL);
	opcion_t* opcion_estado = crear_opcion(titulo_uno_estado, titulo_dos_estado, "", NULL);
	opcion_t* opcion_activar = crear_opcion(titulo_uno_activar, titulo_dos_activar, "", NULL);
	opcion_t* opcion_mostrar = crear_opcion(titulo_uno_mostrar, titulo_dos_mostrar, "", NULL);
	opcion_t* opcion_listar = crear_opcion(titulo_uno_listar, titulo_dos_listar, "", NULL);
	opcion_t* opcion_dest = crear_opcion(titulo_uno_destruir, titulo_dos_destruir, "", salir);

	insertar_opcion_menu(menu, opcion_salir);
	insertar_opcion_menu(menu, opcion_ayuda);
	insertar_opcion_menu(menu, opcion_cargar);
	insertar_opcion_menu(menu, opcion_estado);
	insertar_opcion_menu(menu, opcion_activar);
	insertar_opcion_menu(menu, opcion_mostrar);
	insertar_opcion_menu(menu, opcion_listar);
	insertar_opcion_menu(menu, opcion_dest);

	char op[MAX_STRING] = "";
	bool continuar = true;
	opcion_t* opcion_actual;
	menu_mostrar_opciones(menu);

	while (continuar)
	{
		fgets(op, MAX_STRING, stdin);
		opcion_actual = menu_seleccionar_opcion(menu, op);
		if (opcion_actual->funcion == NULL)
		{
			continuar = false;
		}
		
	}
	
	destruir_menu(menu);

	return 0;
}
