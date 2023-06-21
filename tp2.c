#include "./menu/menu.c"
#include "./src/tp1.h"

#define MAX_STRING 20

typedef struct {
	lista_t *lista;
	void *hospital;
} datos_t;

bool terminar_programa(void *menu, void *contexto)
{
	datos_t *datos = *(datos_t **)contexto;
	lista_iterador_t *iterador = lista_iterador_crear(datos->lista);
	hospital_t *hospital_actual = lista_iterador_elemento_actual(iterador);

	while (hospital_actual) {
		hospital_destruir(hospital_actual);
		lista_iterador_avanzar(iterador);
		hospital_actual = lista_iterador_elemento_actual(iterador);
	}
	lista_iterador_destruir(iterador);
	lista_destruir(datos->lista);
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

bool listar_pokemones(void *menu, void *contexto)
{
	datos_t *datos = *(datos_t **)contexto;
	hospital_t *hospital = datos->hospital;

	if (!hospital) {
		printf("\nNo hay hospitales activos.\n");
		return true;
	}

	printf("\nInformacion detallada de todos los pokemones almacenados en el hospital activo:\n\n");
	size_t cantidad_pokemones = hospital_cantidad_pokemones(hospital);
	size_t cantidad_recorridos =
		hospital_a_cada_pokemon(hospital, listar_pokemones_aux, NULL);

	if (cantidad_pokemones != cantidad_recorridos)
		return false;
	return true;
}

bool mostrar_pokemones_aux(pokemon_t *pokemon, void *aux)
{
	if (!pokemon)
		return false;
	printf("%s.\n", pokemon_nombre(pokemon));
	return true;
}

bool mostrar_pokemones(void *menu, void *contexto)
{
	datos_t *datos = *(datos_t **)contexto;
	hospital_t *hospital = (hospital_t *)datos->hospital;

	if (!hospital) {
		printf("\nNo hay hospitales activos.\n");
		return true;
	}

	printf("\nNombre de todos los pokemones almacenados en el hospital activo:\n");

	size_t cantidad_pokemones = hospital_cantidad_pokemones(hospital);
	size_t cantidad_recorridos =
		hospital_a_cada_pokemon(hospital, mostrar_pokemones_aux, NULL);

	if (cantidad_pokemones != cantidad_recorridos)
		return false;
	return true;
}

bool mostrar_hospitales(void *menu, void *contexto)
{
	datos_t *datos = *(datos_t **)contexto;

	if (!datos)
		return false;

	hospital_t *hospital_actual;
	lista_iterador_t *iterador = lista_iterador_crear(datos->lista);
	int posicion = 0;

	hospital_actual = lista_iterador_elemento_actual(iterador);

	printf("\n\nHospitales cargados:\n");

	while (hospital_actual != NULL) {
		printf("\nID: %d. Nombre: %s.\n", posicion,
		       hospital_nombre(hospital_actual));
		lista_iterador_avanzar(iterador);
		hospital_actual = lista_iterador_elemento_actual(iterador);

		posicion++;
	}

	if (datos->hospital != NULL)
		printf("\nHospital activo: %s.\n",
		       hospital_nombre(datos->hospital));

	lista_iterador_destruir(iterador);
	return true;
}

bool cargar_hospital(void *menu, void *contexto)
{
	char nombre_archivo[MAX_STRING];
	printf("\nIngrese el nombre del archivo con los datos del hospital a cargar: ");

	fgets(nombre_archivo, MAX_STRING, stdin);

	nombre_archivo[strlen(nombre_archivo) - 1] = '\0';
	hospital_t *hospital_nuevo =
		hospital_crear_desde_archivo(nombre_archivo);

	if (!hospital_nuevo) {
		printf("Fallo al cargar el archivo.\n");
		return true;
	}

	datos_t *datos = *(datos_t **)contexto;
	lista_insertar(datos->lista, hospital_nuevo);

	printf("\nHospital cargado con exito.\n");

	return true;
}

bool activar_hospital(void *menu, void *contexto)
{
	datos_t *datos = *(datos_t **)contexto;

	if (!datos)
		return false;

	if (lista_vacia(datos->lista)) {
		printf("\nNo hay hospitales cargados.\n");
		return true;
	}
	mostrar_hospitales(NULL, contexto);
	printf("\nIngrese un ID para activar un hospital: ");

	size_t respuesta;
	scanf("%lu", &respuesta);
	fflush(stdin);

	datos->hospital = lista_elemento_en_posicion(datos->lista, respuesta);

	return true;
}

bool destruir_hospital(void *menu, void *contexto)
{
	datos_t *datos = *(datos_t **)contexto;

	if (!datos->hospital)
		return false;

	lista_iterador_t *iterador = lista_iterador_crear(datos->lista);
	size_t posicion = 0;
	hospital_t *hospital_actual = lista_iterador_elemento_actual(iterador);

	while (hospital_actual != datos->hospital) {
		lista_iterador_avanzar(iterador);
		hospital_actual =
			(hospital_t *)lista_iterador_avanzar(iterador);
		posicion++;
	}

	lista_quitar_de_posicion(datos->lista, posicion);

	hospital_destruir(datos->hospital);
	lista_iterador_destruir(iterador);
	datos->hospital = NULL;

	printf("\nHospital destruido con exito.\n");

	return true;
}

void manejar_opciones(menu_t *menu)
{
	bool continuar = true;
	bool primer_ingreso = true;
	char buffer[MAX_STRING];

	datos_t *datos = malloc(sizeof(datos_t));
	datos->lista = lista_crear();
	datos->hospital = NULL;

	while (continuar) {
		if (!primer_ingreso) {
			printf("\nPresione cualquier tecla para continuar.\n");
			getc(stdin);
		} else {
			primer_ingreso = false;
		}
		menu_mostrar_opciones(menu);
		printf("\n--> ");
		fgets(buffer, MAX_STRING, stdin);
		continuar = menu_seleccionar_opcion(menu, buffer, &datos);
	}
	free(datos);
}

int main()
{
	menu_t *menu = crear_menu();

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
