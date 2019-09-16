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
	bool occupied;
	unsigned int * pBD0; //apuntador que apunta a 12 bloques directos creo.
	unsigned int* pBD1;
	unsigned int* pBD2;
	unsigned int* pBD3;
	unsigned int* pBD4;
	unsigned int* pBD5;
	unsigned int* pBD6;
	unsigned int* pBD7;
	unsigned int* pBD8;
	unsigned int* pBD9;
	unsigned int* pBD10;
	unsigned int* pBD11;
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
		type = '-';
		strcpy_s(fecha_creacion, "null");
		occupied = 0;
	}
};

struct BM {
	char* bm;
};

struct bloqueDirecto {
	char BD[4096]; //4k
	unsigned int numBloque;
	bloqueDirecto() {
		numBloque = -1;
		for (unsigned int i = 0; i < 4096; i++)
			BD[i] = '~';
	}
};

struct bloqueIndirecto1 {//un bloque indirecto 1 nivel apunta a 16 bloques directos.
	bloqueDirecto* pBI1_0;
	bloqueDirecto* pBI1_1;
	bloqueDirecto* pBI1_2;
	bloqueDirecto* pBI1_3;
	bloqueDirecto* pBI1_4;
	bloqueDirecto* pBI1_5;
	bloqueDirecto* pBI1_6;
	bloqueDirecto* pBI1_7;
	bloqueDirecto* pBI1_8;
	bloqueDirecto* pBI1_9;
	bloqueDirecto* pBI1_10;
	bloqueDirecto* pBI1_11;
	bloqueDirecto* pBI1_12;
	bloqueDirecto* pBI1_13;
	bloqueDirecto* pBI1_14;
	bloqueDirecto* pBI1_15;
	bloqueIndirecto1() {
		pBI1_0 = nullptr;
		pBI1_1 = nullptr; 
		pBI1_2 = nullptr;
		pBI1_3 = nullptr;
		pBI1_4 = nullptr;
		pBI1_5 = nullptr;
		pBI1_6 = nullptr;
		pBI1_7 = nullptr;
		pBI1_8 = nullptr;
		pBI1_9 = nullptr;
		pBI1_10 = nullptr;
		pBI1_11 = nullptr;
		pBI1_12 = nullptr;
		pBI1_13 = nullptr;
		pBI1_14 = nullptr;
		pBI1_15 = nullptr;

	}
};

struct bloqueIndirecto2 {//1 bi2 apunta a 32 bi1.
	bloqueIndirecto1* pBI2_0;
	bloqueIndirecto1* pBI2_1;
	bloqueIndirecto1* pBI2_2;
	bloqueIndirecto1* pBI2_3;
	bloqueIndirecto1* pBI2_4;
	bloqueIndirecto1* pBI2_5;
	bloqueIndirecto1* pBI2_6;
	bloqueIndirecto1* pBI2_7;
	bloqueIndirecto1* pBI2_8;
	bloqueIndirecto1* pBI2_9;
	bloqueIndirecto1* pBI2_10;
	bloqueIndirecto1* pBI2_11;
	bloqueIndirecto1* pBI2_12;
	bloqueIndirecto1* pBI2_13;
	bloqueIndirecto1* pBI2_14;
	bloqueIndirecto1* pBI2_15;
	bloqueIndirecto1* pBI2_16;
	bloqueIndirecto1* pBI2_17;
	bloqueIndirecto1* pBI2_18;
	bloqueIndirecto1* pBI2_19;
	bloqueIndirecto1* pBI2_20;
	bloqueIndirecto1* pBI2_21;
	bloqueIndirecto1* pBI2_22;
	bloqueIndirecto1* pBI2_23;
	bloqueIndirecto1* pBI2_24;
	bloqueIndirecto1* pBI2_25;
	bloqueIndirecto1* pBI2_26;
	bloqueIndirecto1* pBI2_27;
	bloqueIndirecto1* pBI2_28;
	bloqueIndirecto1* pBI2_29;
	bloqueIndirecto1* pBI2_30;
	bloqueIndirecto1* pBI2_31;
	bloqueIndirecto2() {
		pBI2_1 = nullptr;
		pBI2_2 = nullptr;
		pBI2_3 = nullptr;
		pBI2_4 = nullptr;
		pBI2_5 = nullptr;
		pBI2_6 = nullptr;
		pBI2_7 = nullptr;
		pBI2_8 = nullptr;
		pBI2_9 = nullptr;
		pBI2_10 = nullptr;
		pBI2_11 = nullptr;
		pBI2_12 = nullptr;
		pBI2_13 = nullptr;
		pBI2_14 = nullptr;
		pBI2_15 = nullptr;
		pBI2_16 = nullptr;
		pBI2_17 = nullptr;
		pBI2_18 = nullptr;
		pBI2_19 = nullptr;
		pBI2_20 = nullptr;
		pBI2_21 = nullptr;
		pBI2_22 = nullptr;
		pBI2_23 = nullptr;
		pBI2_24 = nullptr;
		pBI2_25 = nullptr;
		pBI2_26 = nullptr;
		pBI2_27 = nullptr;
		pBI2_28 = nullptr;
		pBI2_29 = nullptr;
		pBI2_30 = nullptr;
		pBI2_31 = nullptr;

	}

};

struct bloqueIndirecto3 {//un bloque de 3 nivel apunta a 64 bloques indirectos de 2 nivel.
	bloqueIndirecto2* pBI3_0;
	bloqueIndirecto2* pBI3_1;
	bloqueIndirecto2* pBI3_2;
	bloqueIndirecto2* pBI3_3;
	bloqueIndirecto2* pBI3_4;
	bloqueIndirecto2* pBI3_5;
	bloqueIndirecto2* pBI3_6;
	bloqueIndirecto2* pBI3_7;
	bloqueIndirecto2* pBI3_8;
	bloqueIndirecto2* pBI3_9;
	bloqueIndirecto2* pBI3_10;
	bloqueIndirecto2* pBI3_11;
	bloqueIndirecto2* pBI3_12;
	bloqueIndirecto2* pBI3_13;
	bloqueIndirecto2* pBI3_14;
	bloqueIndirecto2* pBI3_15;
	bloqueIndirecto2* pBI3_16;
	bloqueIndirecto2* pBI3_17;
	bloqueIndirecto2* pBI3_18;
	bloqueIndirecto2* pBI3_19;
	bloqueIndirecto2* pBI3_20;
	bloqueIndirecto2* pBI3_21;
	bloqueIndirecto2* pBI3_22;
	bloqueIndirecto2* pBI3_23;
	bloqueIndirecto2* pBI3_24;
	bloqueIndirecto2* pBI3_25;
	bloqueIndirecto2* pBI3_26;
	bloqueIndirecto2* pBI3_27;
	bloqueIndirecto2* pBI3_28;
	bloqueIndirecto2* pBI3_29;
	bloqueIndirecto2* pBI3_30;
	bloqueIndirecto2* pBI3_31;
	bloqueIndirecto2* pBI3_32;
	bloqueIndirecto2* pBI3_33;
	bloqueIndirecto2* pBI3_34;
	bloqueIndirecto2* pBI3_35;
	bloqueIndirecto2* pBI3_36;
	bloqueIndirecto2* pBI3_37;
	bloqueIndirecto2* pBI3_38;
	bloqueIndirecto2* pBI3_39;
	bloqueIndirecto2* pBI3_40;
	bloqueIndirecto2* pBI3_41;
	bloqueIndirecto2* pBI3_42;
	bloqueIndirecto2* pBI3_43;
	bloqueIndirecto2* pBI3_44;
	bloqueIndirecto2* pBI3_45;
	bloqueIndirecto2* pBI3_46;
	bloqueIndirecto2* pBI3_47;
	bloqueIndirecto2* pBI3_48;
	bloqueIndirecto2* pBI3_49;
	bloqueIndirecto2* pBI3_50;
	bloqueIndirecto2* pBI3_51;
	bloqueIndirecto2* pBI3_52;
	bloqueIndirecto2* pBI3_53;
	bloqueIndirecto2* pBI3_54;
	bloqueIndirecto2* pBI3_55;
	bloqueIndirecto2* pBI3_56;
	bloqueIndirecto2* pBI3_57;
	bloqueIndirecto2* pBI3_58;
	bloqueIndirecto2* pBI3_59;
	bloqueIndirecto2* pBI3_60;
	bloqueIndirecto2* pBI3_61;
	bloqueIndirecto2* pBI3_62;
	bloqueIndirecto2* pBI3_63;
	bloqueIndirecto3() {
		pBI3_0 = nullptr;
		pBI3_1 = nullptr;
		pBI3_2 = nullptr;
		pBI3_3 = nullptr;
		pBI3_4 = nullptr;
		pBI3_5 = nullptr;
		pBI3_6 = nullptr;
		pBI3_7 = nullptr;
		pBI3_8 = nullptr;
		pBI3_9 = nullptr;
		pBI3_10 = nullptr;
		pBI3_11 = nullptr;
		pBI3_12 = nullptr;
		pBI3_13 = nullptr;
		pBI3_14 = nullptr;
		pBI3_15 = nullptr;
		pBI3_16 = nullptr;
		pBI3_17 = nullptr;
		pBI3_18 = nullptr;
		pBI3_19 = nullptr;
		pBI3_20 = nullptr;
		pBI3_21 = nullptr;
		pBI3_22 = nullptr;
		pBI3_23 = nullptr;
		pBI3_24 = nullptr;
		pBI3_25 = nullptr;
		pBI3_26 = nullptr;
		pBI3_27 = nullptr;
		pBI3_28 = nullptr;
		pBI3_29 = nullptr;
		pBI3_30 = nullptr;
		pBI3_31 = nullptr;
		pBI3_32 = nullptr;
		pBI3_33 = nullptr;
		pBI3_34 = nullptr;
		pBI3_35 = nullptr;
		pBI3_36 = nullptr;
		pBI3_37 = nullptr;
		pBI3_38 = nullptr;
		pBI3_39 = nullptr;
		pBI3_40 = nullptr;
		pBI3_41 = nullptr;
		pBI3_42 = nullptr;
		pBI3_43 = nullptr;
		pBI3_44 = nullptr;
		pBI3_45 = nullptr;
		pBI3_46 = nullptr;
		pBI3_47 = nullptr;
		pBI3_48 = nullptr;
		pBI3_49 = nullptr;
		pBI3_50 = nullptr;
		pBI3_51 = nullptr;
		pBI3_52 = nullptr;
		pBI3_53 = nullptr;
		pBI3_54 = nullptr;
		pBI3_55 = nullptr;
		pBI3_56 = nullptr;
		pBI3_57 = nullptr;
		pBI3_58 = nullptr;
		pBI3_59 = nullptr;
		pBI3_60 = nullptr;
		pBI3_61 = nullptr;
		pBI3_62 = nullptr;
		pBI3_63 = nullptr;
	}

};
