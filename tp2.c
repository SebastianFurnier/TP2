#include "./menu/menu.c"
#include "./src/tp1.h"

#define MAX_STRING 20

/**
 * Este Struct almacena una lista de hospitales y su posicion en ella es el ID que se usa para activar los hospitales.
 * El puntero hospital_t es el hospital activo.
*/
typedef struct {
	lista_t *hospitales_cargados;
	hospital_t *hospital_activo;
	size_t id_hospital_activo;
} registro_hospitales_t;

typedef struct {
	hospital_t *hospital;
	char nombre[MAX_STRING];
} datos_hospital_t;

bool terminar_programa(void *menu, void *hospitales)
{
	registro_hospitales_t *contenedor_hospitales =
		*(registro_hospitales_t **)hospitales;

	lista_iterador_t *iterador = lista_iterador_crear(
		contenedor_hospitales->hospitales_cargados);

	if (!iterador) {
		printf("\nError al cerrar el programa.\n");
		return false;
	}

	datos_hospital_t *hospital_actual =
		lista_iterador_elemento_actual(iterador);

	while (hospital_actual) {
		hospital_destruir(hospital_actual->hospital);
		free(hospital_actual);
		lista_iterador_avanzar(iterador);
		hospital_actual = lista_iterador_elemento_actual(iterador);
	}

	lista_iterador_destruir(iterador);
	lista_destruir(contenedor_hospitales->hospitales_cargados);
	contenedor_hospitales->hospitales_cargados = NULL;

	printf("\nAdios.\n");

	return false;
}

bool listar_pokemones_aux(pokemon_t *pokemon, void *aux)
{
	if (!pokemon)
		return false;

	printf("Nombre: %s.\n", pokemon_nombre(pokemon));
	printf("ID: %lu.\n", pokemon_id(pokemon));
	printf("Salud: %lu.\n", pokemon_salud(pokemon));
	printf("Entrenador: %s.\n\n", pokemon_entrenador(pokemon));

	return true;
}

bool listar_pokemones(void *menu, void *hospitales)
{
	registro_hospitales_t *contenedor_hospitales =
		*(registro_hospitales_t **)hospitales;

	datos_hospital_t *hospital_actual =
		(datos_hospital_t *)contenedor_hospitales->hospital_activo;

	if (!hospital_actual) {
		printf("\nNo hay hospitales activos.\n");
		return false;
	}

	printf("\nInformacion detallada de todos los pokemones almacenados en el hospital activo:\n\n");

	size_t cantidad_pokemones =
		hospital_cantidad_pokemones(hospital_actual->hospital);

	size_t cantidad_recorridos = hospital_a_cada_pokemon(
		hospital_actual->hospital, listar_pokemones_aux, NULL);

	if (cantidad_pokemones != cantidad_recorridos) {
		printf("\nError al listar los pokemones.\n");
		return false;
	}

	return true;
}

bool mostrar_pokemones_aux(pokemon_t *pokemon, void *aux)
{
	if (!pokemon)
		return false;

	printf("%s.\n", pokemon_nombre(pokemon));

	return true;
}

bool mostrar_pokemones(void *menu, void *hospitales)
{
	registro_hospitales_t *contenedor_hospitales =
		*(registro_hospitales_t **)hospitales;

	datos_hospital_t *hospital_activo_aux =
		(datos_hospital_t *)contenedor_hospitales->hospital_activo;

	if (!hospital_activo_aux) {
		printf("\nNo hay hospitales activos.\n");
		return false;
	}

	printf("\nNombre de todos los pokemones almacenados en el hospital activo:\n");

	size_t cantidad_pokemones =
		hospital_cantidad_pokemones(hospital_activo_aux->hospital);

	size_t cantidad_recorridos = hospital_a_cada_pokemon(
		hospital_activo_aux->hospital, mostrar_pokemones_aux, NULL);

	if (cantidad_pokemones != cantidad_recorridos) {
		printf("\nError al mostrar los pokemones.\n");
		return false;
	}

	return true;
}

bool mostrar_hospitales(void *menu, void *hospitales)
{
	registro_hospitales_t *contenedor_hospitales =
		*(registro_hospitales_t **)hospitales;

	if (!contenedor_hospitales)
		return false;

	datos_hospital_t *hospital_actual;
	lista_iterador_t *iterador = lista_iterador_crear(
		contenedor_hospitales->hospitales_cargados);

	int posicion = 0;

	hospital_actual = lista_iterador_elemento_actual(iterador);

	printf("\n\nHospitales cargados:\n");

	while (hospital_actual) {
		printf("\nID: %d. Nombre: %s.\n", posicion,
		       hospital_actual->nombre);

		lista_iterador_avanzar(iterador);
		hospital_actual = lista_iterador_elemento_actual(iterador);

		posicion++;
	}

	datos_hospital_t *hospital_activo_aux =
		(datos_hospital_t *)contenedor_hospitales->hospital_activo;

	if (hospital_activo_aux)
		printf("\nHospital activo: %s.\n", hospital_activo_aux->nombre);

	lista_iterador_destruir(iterador);
	return true;
}

bool cargar_hospital(void *menu, void *hospitales)
{
	char nombre_archivo[MAX_STRING];
	printf("\nIngrese el nombre del archivo con los datos del hospital a cargar: ");

	fgets(nombre_archivo, MAX_STRING, stdin);

	nombre_archivo[strlen(nombre_archivo) - 1] = '\0';

	hospital_t *hospital_nuevo =
		hospital_crear_desde_archivo(nombre_archivo);

	if (!hospital_nuevo) {
		printf("Fallo al cargar el archivo.\n");
		return false;
	}

	datos_hospital_t *hospital_nuevo_datos =
		malloc(sizeof(datos_hospital_t));

	if (!hospital_nuevo_datos) {
		hospital_destruir(hospital_nuevo);
		return false;
	}

	hospital_nuevo_datos->hospital = hospital_nuevo;
	strcpy(hospital_nuevo_datos->nombre, nombre_archivo);

	registro_hospitales_t *contenedor_hospitales =
		*(registro_hospitales_t **)hospitales;

	lista_insertar(contenedor_hospitales->hospitales_cargados,
		       hospital_nuevo_datos);

	printf("\nHospital cargado con exito.\n");

	return true;
}

bool activar_hospital(void *menu, void *hospitales)
{
	registro_hospitales_t *contenedor_hospitales =
		*(registro_hospitales_t **)hospitales;

	if (!contenedor_hospitales)
		return false;

	if (lista_vacia(contenedor_hospitales->hospitales_cargados)) {
		printf("\nNo hay hospitales cargados.\n");
		return false;
	}
	mostrar_hospitales(NULL, hospitales);
	printf("\nIngrese un ID para activar un hospital: ");

	size_t respuesta;
	scanf("%lu", &respuesta);
	fflush(stdin);

	contenedor_hospitales->hospital_activo = lista_elemento_en_posicion(
		contenedor_hospitales->hospitales_cargados, respuesta);

	contenedor_hospitales->id_hospital_activo = respuesta;

	return true;
}

bool destruir_hospital(void *menu, void *hospitales)
{
	registro_hospitales_t *contenedor_hospitales =
		*(registro_hospitales_t **)hospitales;

	if (!contenedor_hospitales)
		return false;

	if (!contenedor_hospitales->hospital_activo) {
		printf("\nNo hay hospitales activos.\n");
		return false;
	}

	datos_hospital_t *hospital_activo_aux =
		(datos_hospital_t *)lista_quitar_de_posicion(
			contenedor_hospitales->hospitales_cargados,
			contenedor_hospitales->id_hospital_activo);

	hospital_destruir(hospital_activo_aux->hospital);
	free(hospital_activo_aux);
	contenedor_hospitales->hospital_activo = NULL;

	printf("\nHospital destruido con exito.\n");

	return true;
}

void manejar_opciones(menu_t *menu)
{
	bool primer_ingreso = true;

	printf("		Bienvenido al administrador de hospitales.\n\n");

	char buffer[MAX_STRING];

	registro_hospitales_t *contenedor_hospitales =
		malloc(sizeof(registro_hospitales_t));

	if (!contenedor_hospitales)
		return;

	contenedor_hospitales->hospitales_cargados = lista_crear();
	contenedor_hospitales->hospital_activo = NULL;

	if (!contenedor_hospitales->hospitales_cargados) {
		free(contenedor_hospitales);
		return;
	}

	while (contenedor_hospitales->hospitales_cargados) {
		if (!primer_ingreso) {
			printf("\n			Presione Enter para continuar. ");
			getchar();
		} else {
			primer_ingreso = false;
		}

		menu_mostrar_opciones(menu, true);

		printf("\n--> ");
		fgets(buffer, MAX_STRING, stdin);

		menu_seleccionar_opcion(menu, buffer, &contenedor_hospitales);
	}
	free(contenedor_hospitales);
}

int main()
{
	menu_t *menu = crear_menu();

	if (!menu)
		return -1;

	char *titulo_uno_salir = "S";
	char *titulo_dos_salir = "salir";
	char *descripcion_salir = "Cierra el programa.";

	char *titulo_uno_ayuda = "H";
	char *titulo_dos_ayuda = "ayuda";
	char *descripcion_ayuda = "Muestra la ayuda brindada por el sistema.";

	char *titulo_uno_cargar = "C";
	char *titulo_dos_cargar = "cargar";
	char *descripcion_cargar =
		"Crea un hospital a partir del nombre de archivo ingresado.";

	char *titulo_uno_estado = "E";
	char *titulo_dos_estado = "estado";
	char *descripcion_estado =
		"Muestra un listado con los hospitales cargados y activos.";

	char *titulo_uno_activar = "A";
	char *titulo_dos_activar = "Activar";
	char *descripcion_activar =
		"Activa un hospital a partir del numero de identificacion ingresado.";

	char *titulo_uno_mostrar = "M";
	char *titulo_dos_mostrar = "mostrar";
	char *descripcion_mostrar =
		"Muestra un listado con los nombres de todos los pokemones en el hospital.";

	char *titulo_uno_listar = "L";
	char *titulo_dos_listar = "listar";
	char *descripcion_listar =
		"Muestra un listado detallado de todos los pokemones en el hospital.";

	char *titulo_uno_destruir = "D";
	char *titulo_dos_destruir = "destruir";
	char *descripcion_destruir = "Destruye el hospital activo.";

	crear_opcion(menu, titulo_uno_salir, titulo_dos_salir,
		     descripcion_salir, terminar_programa);
	crear_opcion(menu, titulo_uno_ayuda, titulo_dos_ayuda,
		     descripcion_ayuda, menu_mostrar_descripcion);
	crear_opcion(menu, titulo_uno_cargar, titulo_dos_cargar,
		     descripcion_cargar, cargar_hospital);
	crear_opcion(menu, titulo_uno_estado, titulo_dos_estado,
		     descripcion_estado, mostrar_hospitales);
	crear_opcion(menu, titulo_uno_activar, titulo_dos_activar,
		     descripcion_activar, activar_hospital);
	crear_opcion(menu, titulo_uno_mostrar, titulo_dos_mostrar,
		     descripcion_mostrar, mostrar_pokemones);
	crear_opcion(menu, titulo_uno_listar, titulo_dos_listar,
		     descripcion_listar, listar_pokemones);
	crear_opcion(menu, titulo_uno_destruir, titulo_dos_destruir,
		     descripcion_destruir, destruir_hospital);

	manejar_opciones(menu);

	menu_destruir(menu);

	return 0;
}
