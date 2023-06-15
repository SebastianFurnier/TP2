#include "src/tp1.h"
#include "./menu/menu.c"
#include "./menu/lista.h"

int main()
{
	menu_t* menu = crear_menu();
	destruir_menu(menu);

	return 0;
}
