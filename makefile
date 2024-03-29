VALGRIND_FLAGS=--leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes
CFLAGS =-std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0 -g
CC = gcc

all: clean valgrind-chanutron tp2

valgrind-alumno: compilar-pruebas
	valgrind $(VALGRIND_FLAGS) ./pruebas

compilar-pruebas:
	$(CC) $(CFLAGS) ./pruebas_alumno.c ./menu/*.c ./src/*c -o pruebas

correr-valgrind: compilar
	valgrind $(VALGRIND_FLAGS) ./tp2

correr:
	./tp2

compilar:
	$(CC) $(CFLAGS) ./tp2.c ./menu/lista.c ./src/*.c -o tp2

valgrind-chanutron: pruebas_chanutron
	valgrind $(VALGRIND_FLAGS) ./pruebas_chanutron

pruebas_chanutron: src/*.c pruebas_chanutron.c
	$(CC) $(CFLAGS) src/*.c pruebas_chanutron.c -o pruebas_chanutron

ejemplo: src/*.c tp2.c
	$(CC) $(CFLAGS) src/*.c tp2.c -o tp2

clean:
	rm -f pruebas_alumno pruebas_chanutron tp2
