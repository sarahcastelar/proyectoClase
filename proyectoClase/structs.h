#include <iostream>
using namespace std;

int contIndice = 0;

struct metadata {
	char nombre[30];
	unsigned int tamano;
	char date[9];
	char owner[30];
	unsigned int cant_entradas;
	unsigned int byteBloques;
	unsigned int bitmap_size;
	unsigned int cantBD;
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
	int cantBloquesUsados;
	int firstBloc;
	int lastBlock;
	bool occupied;
	unsigned int* pBD[12];
	struct bloqueIndirecto1* pBi1;
	struct bloqueIndirecto2* pBi2;
	struct bloqueIndirecto3* pBi3;
	inode() {
		indice = -1;
		padre = -1;
		primerHijo = -1;
		rightBrother = -1;
		strcpy_s(nombre, "no name");
		tamano = -1;
		lastBlock = -1;
		type = '-';
		strcpy_s(fecha_creacion, "null");
		occupied = 0;
		cantBloquesUsados = -1;
	}
};

struct BM {
	char* bm;
};

struct bloqueDirecto {
	char * bloque; //4k
	unsigned int indiceBloque;
	bloqueDirecto() {
		indiceBloque = -1;
		bloque = new char[4096];
		for (unsigned int i = 0; i < 4096; i++)
			bloque[i] = '~';
	}
};

struct bloqueIndirecto1 {//un bloque indirecto 1 nivel apunta a 16 bloques directos.
	bloqueDirecto* pBI1[16];
	bloqueIndirecto1() {
		for (int i = 0; i < 16; i++)
			pBI1[i] = nullptr;
	}
};

struct bloqueIndirecto2 {//1 bi2 apunta a 32 bi1.
	bloqueIndirecto1* pBI2[32];
	bloqueIndirecto2() {
		for (int i = 0; i < 32; i++)
			pBI2[i] = nullptr;
	}
};

struct bloqueIndirecto3 {//un bloque de 3 nivel apunta a 64 bloques indirectos de 2 nivel.
	bloqueIndirecto2* pBI3[64];
	bloqueIndirecto3() {
		for (int i = 0; i < 64; i++)
			pBI3[i] = nullptr;
	}
};
