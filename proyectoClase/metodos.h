#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <iomanip>
#include <conio.h>
#include "structs.h"

class metodos {

private:
	metadata md;
	BM bmA;
	int contIndice = 1, contmk = 1, indices = 0, indiceBloques = 0;
	inode nodoActual;
	string fecha = "";
	char* bitMap;

public:

	char* initBitMap(char* bitMap, int inodeEntries)
	{
		md.cantBD = 33308 * inodeEntries;
		bitMap = new char[md.cantBD / 8];
		for (int i = 0; i < md.cantBD / 8; i++)
		{
			bitMap[i] = 0;
		}
		return bitMap;
	}

	void printBitMap(char* bitMap)
	{
		const int SHIFT = 8 * sizeof(char) - 1;
		const char MASK = 1 << SHIFT;

		for (int i = 0; i < md.cantBD / 7; i++)
		{
			cout << "entra\n";
			char value;
			value = bitMap[i];
			for (int c = 1; c <= SHIFT + 1; c++)
			{
				cout << (value & MASK ? '1' : '0');
				value <<= 1;

				if (c % 8 == 0)
					cout << ' ';
			}
		}
		cout << endl;
	}

	void setOn(char* bitMap, int nBlock)
	{
		int positionByte = nBlock / 8;
		int iniPosition = (nBlock / 8) * 8;

		for (int i = iniPosition, x = 7; i < (positionByte * 8) + 8; i++, x--)
		{
			if (i == nBlock)
			{
				bitMap[positionByte] |= 1 << x;
				break;
			}
		}
	}

	void setOff(char* bitMap, int nBlock)
	{
		int positionByte = nBlock / 8;
		int iniPosition = (nBlock / 8) * 8;

		for (int i = iniPosition, x = 7; i < (positionByte * 8) + 8; i++, x--)
		{
			if (i == nBlock)
			{
				bitMap[positionByte] &= ~(1 << x);
				break;
			}
		}
	}

	inode getNodoActual() {
		return nodoActual;
	}

	void setNodoActual(inode na) {
		nodoActual = na;
	}

	char* getBitMapActual() {
		return bmA.bm;
	}

	void setBitMapActual(char * aux) {
		bmA.bm = aux;
	}

	inode getRaiz() {
		inode temp;
		fstream fileC(md.nombre, ios::in | ios::out | ios::binary);
		if (!fileC) {
			cout << "Error de apertura en el archivo!" << endl;
		}
		int pos = sizeof(metadata) + md.bitmap_size;
		fileC.seekg(pos);
		fileC.read(reinterpret_cast<char*>(&temp), sizeof(temp));
		return temp;
	}

	string getDate() {
		time_t t = time(NULL);
		tm* timePtr = localtime(&t);
		string aux = "";
		char fechaCreacion[9];
		string fecha = "";

		(timePtr->tm_mday < 10) ? aux = "0" + to_string(timePtr->tm_mday) : aux = to_string(timePtr->tm_mday);
		(timePtr->tm_mon < 10) ? aux = aux + "0" + to_string(timePtr->tm_mon + 1) : aux = aux + to_string(timePtr->tm_mon + 1); //ver que pedoss con el, me imprimia 8 en vez de 9
		aux = aux + to_string(timePtr->tm_year + 1900);
		for (size_t i = 0; i < 8; i++)
		{
			fechaCreacion[i] = aux.at(i);
		}
		for (size_t i = 0; i < 8; i++)
		{
			fecha += fechaCreacion[i];
		}
		return fecha;
	}

	inode iniBD(bloqueDirecto bd, inode inodo) {
		bd = add(bd);
		inodo.pBD0 = &bd.numBloque;
		bd = add(bd);
		inodo.pBD2 = &bd.numBloque;
		bd = add(bd);
		inodo.pBD3 = &bd.numBloque;
		bd = add(bd);
		inodo.pBD4 = &bd.numBloque;
		bd = add(bd);
		inodo.pBD5 = &bd.numBloque;
		bd = add(bd);
		inodo.pBD6 = &bd.numBloque;
		bd = add(bd);
		inodo.pBD7 = &bd.numBloque;
		bd = add(bd);
		inodo.pBD8 = &bd.numBloque;
		bd = add(bd);
		inodo.pBD9 = &bd.numBloque;
		bd = add(bd);
		inodo.pBD10 = &bd.numBloque;
		bd = add(bd);
		inodo.pBD11 = &bd.numBloque;
		return inodo;
	}

	bloqueIndirecto1* iniB1(bloqueDirecto bd, bloqueIndirecto1 bi1, inode inodo) { //cada bloque i 1 nivel apunta a 1 bd.
		bd = add(bd);
		bi1.pBI1_0 = &bd;
		bd = add(bd);
		bi1.pBI1_1 = &bd;
		bd = add(bd);
		bi1.pBI1_2 = &bd;
		bd = add(bd);
		bi1.pBI1_3 = &bd;
		bd = add(bd);
		bi1.pBI1_4 = &bd;
		bd = add(bd);
		bi1.pBI1_5 = &bd;
		bd = add(bd);
		bi1.pBI1_6 = &bd;
		bd = add(bd);
		bi1.pBI1_7 = &bd;
		bd = add(bd);
		bi1.pBI1_8 = &bd;
		bd = add(bd);
		bi1.pBI1_9 = &bd;
		bd = add(bd);
		bi1.pBI1_10 = &bd;
		bd = add(bd);
		bi1.pBI1_11 = &bd;
		bd = add(bd);
		bi1.pBI1_12 = &bd;
		bd = add(bd);
		bi1.pBI1_13 = &bd;
		bd = add(bd);
		bi1.pBI1_14 = &bd;
		bd = add(bd);
		bi1.pBI1_15 = &bd;
		inodo.pBi1 = &bi1;
		return inodo.pBi1;
	}

	bloqueIndirecto2* iniB2(bloqueDirecto bd,bloqueIndirecto1 bi1, bloqueIndirecto2 bi2,inode inodo) {//cada bi2nivel apunta a 32 bi1nivel.
		bi2.pBI2_0 = iniB1(bd, bi1, inodo);
		bi2.pBI2_1 = iniB1(bd, bi1, inodo);
		bi2.pBI2_2 = iniB1(bd, bi1, inodo);
		bi2.pBI2_3 = iniB1(bd, bi1, inodo);
		bi2.pBI2_4 = iniB1(bd, bi1, inodo);
		bi2.pBI2_5 = iniB1(bd, bi1, inodo);
		bi2.pBI2_6 = iniB1(bd, bi1, inodo);
		bi2.pBI2_7 = iniB1(bd, bi1, inodo);
		bi2.pBI2_8 = iniB1(bd, bi1, inodo);
		bi2.pBI2_9 = iniB1(bd, bi1, inodo);
		bi2.pBI2_10 = iniB1(bd, bi1, inodo);
		bi2.pBI2_11 = iniB1(bd, bi1, inodo);
		bi2.pBI2_12 = iniB1(bd, bi1, inodo);
		bi2.pBI2_13 = iniB1(bd, bi1, inodo);
		bi2.pBI2_14 = iniB1(bd, bi1, inodo);
		bi2.pBI2_15 = iniB1(bd, bi1, inodo);
		bi2.pBI2_16 = iniB1(bd, bi1, inodo);
		bi2.pBI2_17 = iniB1(bd, bi1, inodo);
		bi2.pBI2_18 = iniB1(bd, bi1, inodo);
		bi2.pBI2_19 = iniB1(bd, bi1, inodo);
		bi2.pBI2_20 = iniB1(bd, bi1, inodo);
		bi2.pBI2_21 = iniB1(bd, bi1, inodo);
		bi2.pBI2_22 = iniB1(bd, bi1, inodo);
		bi2.pBI2_23 = iniB1(bd, bi1, inodo);
		bi2.pBI2_24 = iniB1(bd, bi1, inodo);
		bi2.pBI2_25 = iniB1(bd, bi1, inodo);
		bi2.pBI2_26 = iniB1(bd, bi1, inodo);
		bi2.pBI2_27 = iniB1(bd, bi1, inodo);
		bi2.pBI2_28 = iniB1(bd, bi1, inodo);
		bi2.pBI2_29 = iniB1(bd, bi1, inodo);
		bi2.pBI2_30 = iniB1(bd, bi1, inodo);
		bi2.pBI2_31 = iniB1(bd, bi1, inodo);
		return inodo.pBi2;
	}

	bloqueIndirecto3* iniB3(bloqueDirecto bd, bloqueIndirecto1 bi1, bloqueIndirecto2 bi2, bloqueIndirecto3 bi3, inode inodo) {//UN bloqueIndirecto3nivel tiene 64 bloquesInd de 2do nivel.
		bi3.pBI3_0 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_1 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_2 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_3 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_4 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_5 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_6 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_7 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_8 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_9 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_10 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_11 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_12 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_13 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_14 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_15 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_16 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_17 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_18 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_19 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_20 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_21 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_22 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_23 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_24 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_25 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_26 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_27 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_28 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_29 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_30 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_31 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_32 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_33 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_34 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_35 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_36 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_37 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_38 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_39 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_40 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_41 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_42 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_43 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_44 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_45 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_46 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_47 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_48 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_49 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_50 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_51 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_52 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_53 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_54 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_55 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_56 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_57 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_58 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_59 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_60 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_61 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_62 = iniB2(bd, bi1, bi2, inodo);
		bi3.pBI3_63 = iniB2(bd, bi1, bi2, inodo);
		return inodo.pBi3;
	}

	bloqueDirecto add(bloqueDirecto bd) {
		bd.numBloque = indiceBloques;
		indiceBloques++;
		return bd;
	}

	void createDisc(const char* vfs_name, int inodeEntries) {
		inode inodo;
		fstream fileC(vfs_name, ios::out | ios::app | ios::binary);//ofstream
		if (!fileC) {
			cout << "Error de apertura en el archivo. " << endl;
			return;
		}

		//ESCRIBE METADATA.
		if (fecha == "")
			fecha = getDate();
		strcpy_s(md.nombre, vfs_name);
		strcpy_s(md.date, fecha.c_str());
		strcpy_s(md.owner, "Sarah");
		md.cant_entradas = inodeEntries;
		md.tamano = sizeof(md);
		md.bitmap_size = sizeof(bitMap);
		fileC.write(reinterpret_cast<const char*>(&md), sizeof(md));

		//ESCRIBE BITMAP.
		bitMap = new char[md.cantBD / 8];
		for (int i = 0; i < md.cantBD / 8; i++)
		{
			bitMap[i] = 0;
		}
		setBitMapActual(bitMap);
		size_t len = strlen(bitMap);
		fileC.write((char*)& len, sizeof(len));
		fileC.write(bitMap, len);

		//ESCRIBE INODE ENTRIES. 
		inodo.tamano = sizeof(inodo);
		
		//bloques directos 12.
		bloqueDirecto bd;
		inodo = iniBD(bd, inodo);

		//puntero a bloque indirecto de 1 nivel, que apunta a 16 bloques directos.
		bloqueIndirecto1 bi1;
		inodo.pBi1 = iniB1(bd, bi1, inodo);

		//puntero a bloque indirecto de 2 nivel, que apunta a 32 bloques indirectos 1 nivel
		bloqueIndirecto2 bi2;
		inodo.pBi2 = iniB2(bd, bi1, bi2, inodo);

		//puntero a bloque indirecto de 3 nivel, que apunta a 64 bloques indirectos de 2 nivel
		bloqueIndirecto3 bi3;
		inodo.pBi3 = iniB3(bd, bi1, bi2, bi3, inodo);

		cout << "cantidad de bloques directos al finaal: " << indiceBloques << endl;

		for (size_t i = 0; i < md.cant_entradas; i++) {
			inodo.indice = indices;
			fileC.write(reinterpret_cast<const char*>(&inodo), sizeof(inodo));
			indices++;
		}
		//md.byteBloques = sizeof(metadata) + (sizeof(inode) * md.cant_entradas);
		fileC.close();
		insertRaiz();
		//readFile();
	}

	void readFile() {
		ifstream fileC(md.nombre, ios::in | ios::binary);
		if (!fileC) {
			cout << "Error de aprtura en el archivo!" << endl;
		}

		//LEE METADATA
		fileC.seekg(0, ios::beg);
		metadata struct_md;
		fileC.read(reinterpret_cast<char*>(&struct_md), sizeof(struct_md));
		cout << "--------leyendo metadata------" << endl;
		cout << "Nombre del archivo:" << struct_md.nombre << endl
			<< "Fecha creacion: " << struct_md.date << endl
			<< "Autor: " << struct_md.owner << endl
			<< "Tamano: " << struct_md.tamano << endl
			<< "Inode Entries: " << struct_md.cant_entradas << endl
			<< "BitMap size: " << struct_md.bitmap_size << endl;
		cout << "--------fin leyendo meta------" << endl;

		//LEE BITMAP.
		size_t len = 0;
		fileC.read((char*)&len, sizeof(len));
		fileC.read(bitMap, len);
		bitMap[len] = '\0';
		setBitMapActual(bitMap);
		/*const int SHIFT = 8 * sizeof(char) - 1;
		const char MASK = 1 << SHIFT;

		for (int i = 0; i < md.cantBD / 8; i++)
		{
			char value;
			value = bitMap[i];
			for (int c = 1; c <= SHIFT + 1; c++)
			{
				cout << (value & MASK ? '1' : '0');
				value <<= 1;

				if (c % 8 == 0)
					cout << ' ';
			}
		}*/

		//LEE INODE ENTRIES.
		inode struct_i;
		cout << "--------leyendo inodes------" << endl;
		fileC.read(reinterpret_cast<char*>(&struct_i), sizeof(struct_i));

		while (!fileC.eof()) {
			cout << "Nombre Inode: " << struct_i.nombre << endl
				<< "Fecha creacion: " << struct_i.fecha_creacion << endl
				<< "Tipo: " << struct_i.type << endl
				<< "Tamano: " << struct_i.tamano << endl
				<< "Padre: " << struct_i.padre << endl
				<< "First Child: " << struct_i.primerHijo << endl
				<< "Right brother: " << struct_i.rightBrother << endl
				<< "Indice Inodo: " << struct_i.indice << endl
				<< "Occupied: " << struct_i.occupied << endl;
			cout << "--------next inode------" << endl;
			fileC.read(reinterpret_cast<char*>(&struct_i), sizeof(struct_i));
		}
		cout << "--------fin leyendo------" << endl;
		fileC.close();
	}

	void insertRaiz() {
		inode temp;
		fstream fileC(md.nombre, ios::in | ios::out | ios::binary);
		if (!fileC) {
			cout << "Error de apertura en el archivo!" << endl;
		}
		//el nodo ya esta actualizado
		int pos = sizeof(metadata) + md.bitmap_size;//+ (sizeof(inode) * nodoActual.indice)
		fileC.seekg(pos);
		if (fecha == "")
			fecha = getDate();
		strcpy_s(temp.nombre, "RAIZ");
		temp.type = 'd';
		strcpy_s(temp.fecha_creacion, fecha.c_str());
		temp.padre = -1;
		temp.primerHijo = -1;
		temp.rightBrother = -1;
		temp.indice = 0;
		temp.occupied = 1;
		temp.tamano = sizeof(inode);

		fileC.seekp(pos);
		fileC.write(reinterpret_cast<char*>(&temp), sizeof(temp));
		nodoActual = temp;
		fileC.close();
	}

	int nextAvailable() {
		inode temp;
		fstream fileC(md.nombre, ios::in | ios::out | ios::binary);
		if (!fileC) {
			cout << "Error de apertura en el archivo!" << endl;
		}

		int pos = sizeof(metadata) + md.bitmap_size;
		fileC.seekg(pos);
		fileC.read(reinterpret_cast<char*>(&temp), sizeof(temp));

		while (!fileC.eof()) {
			if (temp.occupied == 0) {
				return temp.indice;
			}
			fileC.read(reinterpret_cast<char*>(&temp), sizeof(temp));
		}
		fileC.close();
		return -1;
	}

	void mkdir(const char* fileName, inode nodoActual) {
		int availabeSpace = nextAvailable();
		if (availabeSpace != -1){
			inode inodeE, temp;
			int pos = -1;

			fstream fileC(md.nombre, ios::in | ios::out | ios::binary);
			if (!fileC) {
				cout << "Error de apertura en el archivo!" << endl;
			}		

			//crea el directorio vacio
			strcpy_s(inodeE.nombre, fileName);
			inodeE.type = 'd';
			strcpy_s(inodeE.fecha_creacion, fecha.c_str());
			inodeE.primerHijo = -1;
			inodeE.indice = availabeSpace;
			inodeE.occupied = 1;
			inodeE.padre = nodoActual.indice;
			inodeE.tamano = sizeof(inode);
			contIndice++;

			//actualiza el rightBrother.	
			if (nodoActual.primerHijo == -1) {
				pos = sizeof(metadata) + md.bitmap_size + ((sizeof(inode)) * nodoActual.indice);
				fileC.seekg(pos);
				fileC.read(reinterpret_cast<char*>(&temp), sizeof(temp));
				temp.primerHijo = inodeE.indice;
				fileC.seekg(pos);
				fileC.write(reinterpret_cast<char*>(&temp), sizeof(temp));
				setNodoActual(temp);
			}
			else {
				pos = sizeof(metadata) + md.bitmap_size + ((sizeof(inode)) * nodoActual.primerHijo);
				fileC.seekg(pos);
				fileC.read(reinterpret_cast<char*>(&temp), sizeof(temp));
				temp.rightBrother = inodeE.indice;

				fileC.seekg(pos);
				fileC.write(reinterpret_cast<char*>(&temp), sizeof(temp));

			}

			//lo actualiza en el archivo
			pos = sizeof(metadata) + md.bitmap_size + ((sizeof(inode)) * inodeE.indice);
			fileC.seekg(pos);
			fileC.write(reinterpret_cast<char*>(&inodeE), sizeof(inodeE));
			fileC.close();
			//readFile();
			contmk++;
		}
		else
			cout << "entries full!" << endl;
	}

	void cd(const char* fileName, bool itsDot) {
		inode temp;
		fstream fileC(md.nombre, ios::in | ios::out | ios::binary);
		if (!fileC) {
			cout << "Error de apertura en el archivo!" << endl;
		}

		int pos = sizeof(metadata) + md.bitmap_size;
		int resultado = -1;
		fileC.seekg(pos);
		fileC.read(reinterpret_cast<char*>(&temp), sizeof(temp));

		while (!fileC.eof()) {
			if (itsDot) {
				//si encontramos al padre del nodoactual. es ese. 
				if (temp.indice == nodoActual.padre) {
					setNodoActual(temp);
					break;
				}
			}
			else {
				char* str = temp.nombre;
				resultado = strcmp(str, fileName);
				if (resultado == 0) {
					setNodoActual(temp);
					break;
				}
			}
			fileC.read(reinterpret_cast<char*>(&temp), sizeof(temp));
		}

		if (resultado == -1 && !itsDot) 
			cout << "File or Directory non existent. " << endl;

		fileC.close();
	}

	void ls(inode nodoA) { //seria el nodo actual. 
		//estoy en un nodo actual. ese nodo tiene un indice. 
		inode aux, aux2;
		fstream fileC(md.nombre, ios::in | ios::out | ios::binary);
		if (!fileC) {
			cout << "Error de apertura en el archivo!" << endl;
		}

		int pos = sizeof(metadata) + md.bitmap_size + (sizeof(inode) * nodoActual.indice);
		fileC.seekg(pos);
		fileC.read(reinterpret_cast<char*>(&aux), sizeof(aux));

		bool puede = true;
		pos = sizeof(metadata) + md.bitmap_size + (sizeof(inode) * nodoActual.primerHijo);
		fileC.seekg(pos);
		fileC.read(reinterpret_cast<char*>(&aux2), sizeof(aux2));
		if (aux2.occupied == 0) {
			puede = false;
		}

		if (puede) {
			if (aux.primerHijo != -1) { //si tiene hijo.. nos movemos a la posicion de ese hijo en
				//el archivo y listamos.

				cout << left << setw(10) << "NAME" << setw(13) << "TYPE"
					<< setw(7) << right << "SIZE" << endl;

				pos = sizeof(metadata) + md.bitmap_size + (sizeof(inode) * aux.primerHijo);
				fileC.seekg(pos);
				fileC.read(reinterpret_cast<char*>(&aux), sizeof(aux));
				cout << left << setw(10) << aux.nombre << setw(13) << aux.type
				<< setw(7) << right << aux.tamano << endl;

				//ver si hay right brothers. 
				while (aux.rightBrother != -1) {
					pos = sizeof(metadata) + md.bitmap_size + (sizeof(inode) * aux.rightBrother);
					fileC.seekg(pos);
					fileC.read(reinterpret_cast<char*>(&aux), sizeof(aux));
					cout << left << setw(10) << aux.nombre << setw(13) << aux.type
						<< setw(7) << right << aux.tamano << endl;
				}
			}
			else
				cout << "Directory empty. \n";
		}
		else
			cout << "Directory empty. \n";
		fileC.close();
	}

	int  findPos(const char* fileName) {
		/*
			FUNCION QUE DEVUELVE EL INDICE DEL NODO A BORRAR.
		*/
		inode temp;
		fstream fileC(md.nombre, ios::in | ios::out | ios::binary);
		if (!fileC) {
			cout << "Error de apertura en el archivo!" << endl;
		}

		int pos = sizeof(metadata) + md.bitmap_size ;
		int resultado = -1;
		fileC.seekg(pos);
		fileC.read(reinterpret_cast<char*>(&temp), sizeof(temp));

		while (!fileC.eof()) {
			char* str = temp.nombre;
			resultado = strcmp(str, fileName);
			cout << "str: " << str << " fileName: " << fileName << endl;
			if (resultado == 0) {
				return temp.indice;
			}
			fileC.read(reinterpret_cast<char*>(&temp), sizeof(temp));
		}
		fileC.close();
		return -1;
	}

	char* findName(int indice) {
		inode temp;
		char* nombre;
		fstream fileC(md.nombre, ios::in | ios::out | ios::binary);
		if (!fileC) {
			cout << "Error de apertura en el archivo!" << endl;
		}

		int pos = sizeof(metadata) + md.bitmap_size + ((sizeof(inode)) * indice);
		fileC.seekg(pos);
		fileC.read(reinterpret_cast<char*>(&temp), sizeof(temp));
		nombre = temp.nombre;
		fileC.close();
		return nombre;
	}

	int findRightBrother(int indice) {
		inode temp;
		fstream fileC(md.nombre, ios::in | ios::out | ios::binary);
		if (!fileC) {
			cout << "Error de apertura en el archivo!" << endl;
		}

		int pos = sizeof(metadata) + md.bitmap_size + ((sizeof(inode)) * indice);
		fileC.seekg(pos);
		fileC.read(reinterpret_cast<char*>(&temp), sizeof(temp));
		return temp.rightBrother;
	}

	int occupied() {
		inode temp;
		fstream fileC(md.nombre, ios::in | ios::out | ios::binary);
		if (!fileC) {
			cout << "Error de apertura en el archivo!" << endl;
		}

		int pos = sizeof(metadata) + md.bitmap_size + ((sizeof(inode)) * nodoActual.indice);
		fileC.seekg(pos);
		fileC.read(reinterpret_cast<char*>(&temp), sizeof(temp));

		pos = sizeof(metadata) + md.bitmap_size + (sizeof(inode) * temp.primerHijo);
		fileC.seekg(pos);
		fileC.read(reinterpret_cast<char*>(&temp), sizeof(temp));
		
		return temp.occupied;
	}

	bool rm(const char* fileName, int indice) {
		int borrar = findPos(fileName);
		bool itsDot = false;
		if (borrar != -1 || (strcmp(fileName, "null") == 0)) {
			inode temp;
			fstream fileC(md.nombre, ios::in | ios::out | ios::binary);
			if (!fileC) {
				cout << "Error de apertura en el archivo!" << endl;
			}

			if (strcmp(fileName, "null") != 0)
				indice = findPos(fileName);

			//si se esta borrando el nodo actual, hay que set el nuevo nodoActual, osea el padree. 
			if (strcmp(fileName, nodoActual.nombre) == 0) {
				cd(fileName, true);
				itsDot = true;
			}

			int pos = sizeof(metadata) + md.bitmap_size + ((sizeof(inode)) * indice);
			int resultado = -1;
			fileC.seekg(pos);
			fileC.read(reinterpret_cast<char*>(&temp), sizeof(temp));

			if (temp.type == 'f') {
				temp.occupied = 0;
				fileC.seekg(pos);
				fileC.write(reinterpret_cast<char*>(&temp), sizeof(temp));
				contIndice--;
			}
			else if ((temp.primerHijo != -1 && temp.rightBrother == -1) || (temp.primerHijo == -1 && temp.rightBrother != -1)) {//si tiene hijos
				temp.occupied = 0;
				fileC.seekg(pos);
				fileC.write(reinterpret_cast<char*>(&temp), sizeof(temp));
				contIndice--;

				//si no tiene hijo pero si rightBrother
				if (temp.primerHijo == -1 && temp.rightBrother != -1) {
					int iRBdelHijo = findRightBrother(temp.indice);
					if (iRBdelHijo != -1)
						rm("null", iRBdelHijo);
				}
				else {//si tiene hijo pero no rightBrother
					rm("null", temp.primerHijo);
				}

			}
			else {
				temp.occupied = 0;
				fileC.seekg(pos);
				fileC.write(reinterpret_cast<char*>(&temp), sizeof(temp));
				contIndice--;
			}

			//saber si se esta borrando el firstC de la raiz. 
			inode raizTemp = getRaiz();
			char* name = findName(raizTemp.primerHijo);
			if ((strcmp(fileName, name) == 0) || (indice == raizTemp.primerHijo))
				setNodoActual(raizTemp);

			fileC.close();
			readFile();
		}
		else
			cout << "Ese directorio/archivo no existe. " << endl;
		return itsDot;
	}

	void import(const char* fileName) {
		/*
			se va leyendo el archivo, se agarran los 4096 bytes 
			y los guardo en algun bloque directo en mi vfs archivo, 
			hasta que todo el archivo se haya leido correctamente,
			actualizo el mapa de bits para poner todos los 
			bloques directos usados en ocupado.
		*/
		fstream fileC(fileName, ios::in | ios::out | ios::binary);
		if (!fileC) {
			cout << "Error de apertura en el archivo!" << endl;
		}

		int pos = sizeof(metadata) + md.bitmap_size;

		fileC.seekg(pos);
		fileC.read(reinterpret_cast<char*>(&temp), sizeof(temp));



		fileC.close();
	}
};