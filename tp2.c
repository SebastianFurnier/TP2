#include "./menu/menu.c"
#include "./menu/hash.h"
#include "./src/tp1.h"


#define MAX_STRING 20

bool terminar_programa(void* menu, void* hospital)
{
	hospital_t* hospital_aux = *(hospital_t**)hospital;
	hospital_destruir(hospital_aux);
	printf("\nAdios.\n");
	return false;
}

bool listar_pokemones_aux(pokemon_t* pokemon, void* aux)
{
	if(!pokemon)
		return false;
	printf("Nombre: %s.\n", pokemon_nombre(pokemon));
	printf("ID: %lu.\n", pokemon_id(pokemon));
	printf("Salud: %lu.\n", pokemon_salud(pokemon));
	printf("Entrenador: %s.\n\n", pokemon_entrenador(pokemon));
	return true;
}

bool listar_pokemones(void* menu, void* contexto)
{
	hospital_t* hospital = *(hospital_t**)contexto;

	if (!hospital)
		return false;
	printf("\nInformacion detallada de todos los pokemones almacenados en el hospital activo:\n\n");
	size_t cantidad_pokemones = hospital_cantidad_pokemones(hospital);
	size_t cantidad_recorridos = hospital_a_cada_pokemon(hospital, listar_pokemones_aux, NULL);

	if (cantidad_pokemones != cantidad_recorridos)
		return false;
	return true;

}

bool mostrar_pokemones_aux(pokemon_t* pokemon, void* aux)
{
	if(!pokemon)
		return false;
	printf("%s.\n", pokemon_nombre(pokemon));
	return true;
}

bool mostrar_pokemones(void* menu, void* contexto)
{	
	hospital_t* hospital = *(hospital_t**)contexto;

	if (!hospital)
		return false;
	printf("\nNombre de todos los pokemones almacenados en el hospital activo:\n");
	size_t cantidad_pokemones = hospital_cantidad_pokemones(hospital);
	size_t cantidad_recorridos = hospital_a_cada_pokemon(hospital, mostrar_pokemones_aux, NULL);

	if (cantidad_pokemones != cantidad_recorridos)
		return false;
	return true;

}

bool cargar_hospital(void* menu, void* contexto_aux)
{
	char nombre_archivo[MAX_STRING];
	printf("\nIngrese el nombre del archivo con los datos del hospital a cargar: ");

	fgets(nombre_archivo, MAX_STRING, stdin);

	nombre_archivo[strlen(nombre_archivo)-1] = '\0';
	hospital_t* hospital_nuevo = hospital_crear_desde_archivo(nombre_archivo);

	if (!hospital_nuevo){
		printf("Fallo al cargar el archivo.\n");
		return false;
	}
	*(hospital_t**)contexto_aux = hospital_nuevo;
	printf("\nHospital cargado con exito.\n");

	return true;
}

void manejar_opciones(menu_t* menu)
{
	bool continuar = true;
	char buffer[MAX_STRING];

	hospital_t* hospital_activo = NULL;
	//hash_t* hash_hospitales = hash_crear(3);

	while (continuar)
	{
		menu_mostrar_opciones(menu);
		printf("\n--> ");
		fgets(buffer, MAX_STRING, stdin);
		continuar = menu_seleccionar_opcion(menu, buffer, &hospital_activo);
	}
}

int main()
{
	menu_t* menu = crear_menu();

	char* titulo_uno_salir = "S";
	char* titulo_dos_salir = "salir";
	char* descripcion_salir = "Cierra el programa.";

	char* titulo_uno_ayuda = "H";
	char* titulo_dos_ayuda = "ayuda";
	char* descripcion_ayuda = "Muestra la ayuda brindada por el sistema.";

	char* titulo_uno_cargar= "C";
	char* titulo_dos_cargar = "cargar";
	char* descripcion_cargar = "Crea un hospital a partir del nombre de archivo ingresado.";

	char* titulo_uno_estado = "E";
	char* titulo_dos_estado = "estado";
	char* descripcion_estado = "Muestra un listado con los hospitales cargados y activos.";

	char* titulo_uno_activar = "A";
	char* titulo_dos_activar = "Activar";
	char* descripcion_activar = "Activa un hospital a partir del numero de identificacion ingresado.";

	char* titulo_uno_mostrar = "M";
	char* titulo_dos_mostrar = "mostrar";
	char* descripcion_mostrar = "Muestra un listado con los nombres de todos los pokemones en el hospital.";

	char* titulo_uno_listar = "L";
	char* titulo_dos_listar = "listar";
	char* descripcion_listar = "Muestra un listado detallado de todos los pokemones en el hospital.";

	char* titulo_uno_destruir = "D";
	char* titulo_dos_destruir = "destruir";
	char* descripcion_destruir = "Destruye el hospital activo.";

	crear_opcion(menu, titulo_uno_salir, titulo_dos_salir, descripcion_salir, terminar_programa);
	crear_opcion(menu, titulo_uno_ayuda, titulo_dos_ayuda, descripcion_ayuda, menu_mostrar_descripcion);
	crear_opcion(menu, titulo_uno_cargar, titulo_dos_cargar, descripcion_cargar, cargar_hospital);
	crear_opcion(menu, titulo_uno_estado, titulo_dos_estado, descripcion_estado, NULL);
	crear_opcion(menu, titulo_uno_activar, titulo_dos_activar, descripcion_activar, NULL);
	crear_opcion(menu, titulo_uno_mostrar, titulo_dos_mostrar, descripcion_mostrar, mostrar_pokemones);
	crear_opcion(menu, titulo_uno_listar, titulo_dos_listar, descripcion_listar, listar_pokemones);
	crear_opcion(menu, titulo_uno_destruir, titulo_dos_destruir, descripcion_destruir, NULL);

	manejar_opciones(menu);

	menu_destruir(menu);

	return 0;
}
