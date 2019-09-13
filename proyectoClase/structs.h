#include <iostream>
using namespace std;

struct metadata {
	char nombre[30];
	unsigned int tamano;
	char date[9];
	char owner[30];
	unsigned int cant_entradas;
	unsigned int bitmap_size;
	//unsigned int cant_bloques_directos;
	metadata() {
		bitmap_size = 0;
		tamano = 0;
		cant_entradas = 10;

	}
};

struct inode {
	int indice;
	char nombre[30];
	char type;
	char fecha_creacion[9];
	int tamano;
	int padre;
	int primerHijo;
	int rightBrother;
	bool occupied;
	//unsigned int bloquesDirectos[12];
	//unsigned int bloquesIndirectos[3];
	inode() {
		indice = -1;
		padre = -1;
		primerHijo = -1;
		rightBrother = -1;
		strcpy_s(nombre, "no name");
		tamano = -1;
		type = '-';
		strcpy_s(fecha_creacion, "null");
		occupied = 0;
	}
};

struct bloqueDirecto {
	char texto[4096]; //4k
	bloqueDirecto() {
		for (int i = 0; i < 4096; i++)
			texto[i] = '~';
	}
};
