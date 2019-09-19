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
	int contIndice = 1, contmk = 1, indices = 1, indiceBloques = 0, contManualBloques = 0, cantBloquesUsados = 1, contNormal = 0;
	inode nodoActual;
	string fecha = "";
	bool first = true, importCreado = false;
	char* bitMap;

public:

	char* initBitMap(char* bitMap)
	{
		int j = 1;
		for (int i = 0; i < md.cantBD / 8; i++)
		{
			bitMap[i] = 0;
			j++;
		}
		return bitMap;
	}

	void printBitMap(char* bitMap)
	{
		int j = 1;
		const int SHIFT = 8 * sizeof(char) - 1;
		const char MASK = 1 << SHIFT;

		for (int i = 0; i < md.cantBD / 7; i++)
		{
			char value;
			value = bitMap[i];
			for (int c = 1; c <= SHIFT + 1; c++)
			{
				j++;
				cout << (value & MASK ? '1' : '0');
				value <<= 1;

				if (c % 8 == 0)
					cout << ' ';
			}
		}
		cout << endl;
	}

	char* setOn(char* bitMap, int nBlock)
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
		return bitMap;
	}

	char* setOff(char* bitMap, int nBlock)
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
		return bitMap;
	}

	inode getNodoActual() {
		return nodoActual;
	}

	void setNodoActual(inode na) {
		nodoActual = na;
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

	inode iniBD(inode inodo) { //INICIALIZAR LOS PRIMEROS 12.
		int numEsperado = 33308 * (md.cant_entradas - 1);
		if (indiceBloques != numEsperado) {
			fstream fileC(md.nombre, ios::in | ios::out | ios::binary);
			if (!fileC) {
				cout << "Error de apertura en el archivo!" << endl;
				return inodo;
			}

			//cada bloque de dato creado se escribe en el archivo too.
			for (size_t i = 0; i < 12; i++)
			{
				bloqueDirecto bd;
				bd.indiceBloque = indiceBloques;
				inodo.pBD[i] = &bd.indiceBloque; //APUNTA AL INDICE. 
				int pos = sizeof(metadata) + md.bitmap_size + (sizeof(inode) * md.cant_entradas) + (sizeof(bd) * indiceBloques);
				fileC.seekg(pos);
				fileC.write(reinterpret_cast<char*>(&bd), sizeof(bd));
				indiceBloques++;
				contManualBloques++;
			}
			fileC.close();
		}
		return inodo;
	}

	bloqueIndirecto1* iniB1(bloqueIndirecto1 bi1, inode inodo) { //cada bloque i 1 nivel apunta a 1 bd.
		int numEsperado = 33308 * (md.cant_entradas - 1);
		if (indiceBloques != numEsperado) {
			fstream fileC(md.nombre, ios::in | ios::out | ios::binary);
			if (!fileC) {
				cout << "Error de apertura en el archivo!" << endl;
				return inodo.pBi1;
			}
			for (int i = 0; i < 16; i++) {
				bloqueDirecto bd;
				bd.indiceBloque = indiceBloques;
				bi1.pBI1[i] = &bd;
				int pos = sizeof(metadata) + md.bitmap_size + (sizeof(inode) * md.cant_entradas) + (sizeof(bd) * indiceBloques);
				fileC.seekg(pos);
				fileC.write(reinterpret_cast<char*>(&bd), sizeof(bd));
				contManualBloques++;
				indiceBloques++;
			}
			fileC.close();
		}
		return inodo.pBi1;
	}

	bloqueIndirecto2* iniB2(bloqueIndirecto2 bi2, inode inodo) 
	{//cada bi2nivel apunta a 32 bi1nivel.
		//un bloque indirecto de 2do nivel apunta a 32 bloques de primer nivel.
		int numEsperado = 33308 * (md.cant_entradas - 1);
		if (indiceBloques != numEsperado) {
			for (int i = 0; i < 32; i++) {
				bloqueIndirecto1 bi1;
				bi2.pBI2[i] = iniB1(bi1, inodo);
			}
		}
		return inodo.pBi2;

	}

	bloqueIndirecto3* iniB3(bloqueIndirecto1 bi1, bloqueIndirecto2 bi2, bloqueIndirecto3 bi3, inode inodo) {//UN bloqueIndirecto3nivel tiene 64 bloquesInd de 2do nivel.
		int numEsperado = 33308 * (md.cant_entradas - 1);
		if (indiceBloques != numEsperado) {
			for (int i = 0; i < 64; i++) {
				bi3.pBI3[i] = iniB2(bi2, inodo);

			}
		}
		return inodo.pBi3;
	}

	inode inicializarNodo(inode inodo) {
		//bloques directos 12.
		inodo = iniBD(inodo);

		//puntero a bloque indirecto de 1 nivel, que apunta a 16 bloques directos.
		bloqueIndirecto1 bi1;
		inodo.pBi1 = iniB1(bi1, inodo);

		//puntero a bloque indirecto de 2 nivel, que apunta a 32 bloques indirectos 1 nivel
		bloqueIndirecto2 bi2;
		inodo.pBi2 = iniB2(bi2, inodo);

		//puntero a bloque indirecto de 3 nivel, que apunta a 64 bloques indirectos de 2 nivel
		bloqueIndirecto3 bi3;
		inodo.pBi3 = iniB3(bi1, bi2, bi3, inodo);
		return inodo;
	}

	void createDisc(const char* vfs_name, int inodeEntries) {
		fstream fileC(vfs_name, ios::out | ios::app | ios::binary);//ofstream
		if (!fileC) {
			cout << "Error de apertura en el archivo. " << endl;
			return;
		}

		//char* bitMap;
		md.cantBD = 33308 * (inodeEntries - 1);
		bitMap = new char[md.cantBD / 8];

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
		bitMap = initBitMap(bitMap);
		size_t len = strlen(bitMap);
		fileC.write((char*)& len, sizeof(len));
		fileC.write(bitMap, len);
		//printBitMap(bitMap);

		//ESCRIBE INODE ENTRIES. 
		inode inodoRaiz;
		inodoRaiz.tamano = sizeof(inode);

		
		for (size_t i = 0; i < md.cant_entradas - 1; i++) {
			if (i == 0) {
				inodoRaiz.indice = indices;
				fileC.write(reinterpret_cast<const char*>(&inodoRaiz), sizeof(inodoRaiz));
			}	
			else {
				inode inodo;
				inodo = inicializarNodo(inodo);
				inodo.tamano = sizeof(inode);
				fileC.write(reinterpret_cast<const char*>(&inodo), sizeof(inodo));
			}
		}

		fileC.close();
		insertRaiz();
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

		inode inodo;

		for (size_t i = 1; i < md.cant_entradas; i++)
		{
			pos = sizeof(metadata) + md.bitmap_size + (sizeof(inode) * i);
			inodo = inicializarNodo(inodo);
			inodo.indice = indices;
			inodo.padre = -1;
			inodo.primerHijo = -1;
			inodo.rightBrother = -1;
			strcpy_s(inodo.nombre, "no name");
			inodo.tamano = -1;
			inodo.lastBlock = -1;
			inodo.type = '-';
			strcpy_s(inodo.fecha_creacion, "null");
			inodo.occupied = 0;
			inodo.cantBloquesUsados = -1;
			indices++;
			fileC.write(reinterpret_cast<char*>(&inodo), sizeof(inodo));
		}
		fileC.close();
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

		fileC.seekg(sizeof(metadata));
		//LEE BITMAP.
		char* bitMap;
		bitMap = new char[md.cantBD / 8];
		size_t len = 0;
		fileC.read((char*)& len, sizeof(len));
		fileC.read(bitMap, len);
		bitMap[len] = '\0';
		//printBitMap(bitMap);

		//LEE INODE ENTRIES.
		inode struct_i;
		cout << "--------leyendo inodes------" << endl;
		fileC.read(reinterpret_cast<char*>(&struct_i), sizeof(struct_i));
		int contBloques = 0;

		for (size_t i = 0; i < md.cant_entradas; i++)
		{
			contBloques = 0;
			cout << "Nombre Inode: " << struct_i.nombre << endl
				<< "Fecha creacion: " << struct_i.fecha_creacion << endl
				<< "Tipo: " << struct_i.type << endl
				<< "Tamano: " << struct_i.tamano << endl
				<< "Padre: " << struct_i.padre << endl
				<< "First Child: " << struct_i.primerHijo << endl
				<< "Right brother: " << struct_i.rightBrother << endl
				<< "Indice Inodo: " << struct_i.indice << endl
				<< "Occupied: " << struct_i.occupied << endl
				<< "Cant. Bloques Usados: " << struct_i.cantBloquesUsados << endl
				<< "Last block: " << struct_i.lastBlock << endl;
			cout << "--------next inode------" << endl;
			fileC.read(reinterpret_cast<char*>(&struct_i), sizeof(struct_i));
		} 	
		cout << "--------fin leyendo------" << endl;

		//LEER BLOQUES DE DATOS.
		/*bloqueDirecto bd;
		int pos = sizeof(metadata) + md.bitmap_size + (sizeof(inode) * md.cant_entradas);
		fileC.seekg(pos);
		fileC.read(reinterpret_cast<char*>(&bd), sizeof(bd));
		for (int i = 0; i < md.cantBD; i++) {
			cout << " Ind: " << bd.indiceBloque;
				//<< "data adentro: " << bd.bloque << endl;
			//cout << endl;
			fileC.read(reinterpret_cast<char*>(&bd), sizeof(bd));
		}*/
		//cout << "cantidad de bloques son: " << md.cantBD << endl;
		//cout << "indices sacados son: " << contManualBloques << endl;

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
		bool foundIt = false;

		for (size_t i = 0; i < md.cant_entradas; i++) {
			if (temp.occupied == 0) {
				fileC.close();
				return temp.indice;
			}

			fileC.read(reinterpret_cast<char*>(&temp), sizeof(temp));
		}
		fileC.close();
		cout << "termina al for y no entra y retorna -1: " << temp.indice << endl;
		return -1;
	}

	void mkdir(const char* fileName, inode nodoActual, bool itsD) {
		int availabeSpace = nextAvailable();
		if (availabeSpace > 0) {
			inode inodeE, temp;
			int pos = -1;

			fstream fileC(md.nombre, ios::in | ios::out | ios::binary);
			if (!fileC) {
				cout << "Error de apertura en el archivo!" << endl;
			}

			if (itsD)
				inodeE.type = 'd';
			else
				inodeE.type = 'f';
			//crea el directorio vacio
			strcpy_s(inodeE.nombre, fileName);
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
			return;
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
		//FUNCION QUE DEVUELVE EL INDICE DE UN NODO.
		inode temp;
		fstream fileC(md.nombre, ios::in | ios::out | ios::binary);
		if (!fileC) {
			cout << "Error de apertura en el archivo!" << endl;
		}

		int pos = sizeof(metadata) + md.bitmap_size;
		int resultado = -1;
		fileC.seekg(pos);
		fileC.read(reinterpret_cast<char*>(&temp), sizeof(temp));

		for (int i = 0; i < md.cant_entradas; i++) {
			char* str = temp.nombre;
			resultado = strcmp(str, fileName);
			if (resultado == 0) {
				fileC.close();
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
				for (size_t i = temp.firstBlock; i < temp.cantBloquesUsados; i++)
				{
					bitMap = getBitMap();
					bitMap = updateBitmap(i, false, bitMap);
					setBitMap(bitMap);
				}
				contNormal = 0;
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
		}
		else
			cout << "Ese directorio/archivo no existe. " << endl;
		return itsDot;
	}

	int nextBitAvailable(char* bitMap) {
		const int SHIFT = 8 * sizeof(char) - 1;
		const char MASK = 1 << SHIFT;
		int n = -1;
		for (int i = 0; i < md.cantBD / 8; i++)
		{
			char value;
			value = bitMap[i];
			for (int c = 1; c <= SHIFT + 1; c++)
			{
				if ((value & MASK) == 0) {
					n = (7 * i) + (c - 1);
					break;
				}
				value <<= 1;
			}
			if (n != -1)
				break;
		}
		return n;
	}

	void import(const char* fileName) {
		ifstream fileC(fileName, ios::in | ios::binary);
		if (!fileC) {
			cout << " en import Error de apertura en el archivo!" << endl;
			return;
		}

		int firstBlock = -1;
		if (cantBloquesUsados == 1) {
			firstBlock = nextBitAvailable(getBitMap());
		}

		
		while (!fileC.eof()) { 
			char * bloque = new char[4096];
			
			fileC.read(bloque, 4096);
			import_1(fileName, bloque);
		}
		bitMap = getBitMap();
		actualizarCantBD(firstBlock, fileName);

		for (size_t i = 0; i < contNormal; i++)
		{
			bitMap = updateBitmap(i, true, bitMap);
			setBitMap(bitMap);
		}
		int n = -2;
		n = nextBitAvailable(bitMap);

		fileC.close();
	}

	void actualizarCantBD(int firstBlock, const char* fileName) {
		fstream fileC(md.nombre, ios::in | ios::out | ios::binary);
		if (!fileC) {
			cout << "Error de apertura en el archivo!" << endl;
			return;
		}

		int indice = findPos(fileName);
		inode nodo;

		int pos = sizeof(metadata) + md.bitmap_size + (sizeof(inode) * indice);
		fileC.seekg(pos);
		fileC.read(reinterpret_cast<char*>(&nodo), sizeof(nodo));

		nodo.cantBloquesUsados = contNormal;
		nodo.firstBlock = nextBitAvailable(getBitMap());

		pos = sizeof(metadata) + md.bitmap_size + (sizeof(inode) * indice);
		fileC.seekg(pos);
		fileC.write(reinterpret_cast<char*>(&nodo), sizeof(nodo));
		fileC.close();
	}

	void funciona(const char* fileName, bloqueDirecto bd) {
		string s(fileName);
		//png, jpeg, doc, mp4
		string commands[3] = { ".png", ".jpeg", ".doc" };
		for (size_t i = 0; i < 3; i++)
		{
			int encontro = s.find(commands[i]);
			if (encontro != string::npos) {
				switch (i) {
				case 0:
					s = s.substr(0, s.length() - 4);
					s = s + "1.png";
					break;
				case 1:
					s = s.substr(0, s.length() - 5);
					s = s + "1.jpeg";
					break;
				case 2:
					s = s.substr(0, s.length() - 4);
					s = s + "1.doc";
					break;
				}
			}
		}

		string s1 = ".mp4";
		int encontro = s.find(s1);
		if (encontro != string::npos) {
			s = s.substr(0, s.length() - 4);
			s = s + "1.mp4";
		}

		ofstream fileC(s, ios::out | ios::app| ios::binary);
		if (!fileC) {
			cout << " en funciona. Error de apertura en el archivo!" << endl;
			return;
		}
		fileC.seekp(0, ios::end);
		fileC.write(bd.bloque, 4096);
		fileC.close();
	}

	char* getBitMap() {
		/*char* bitMap;
		bitMap = new char[md.cantBD / 8];
		ifstream fileC(md.nombre, ios::in | ios::binary);
		if (!fileC) {
			cout << "Error de apertura en el archivo!" << endl;
			return bitMap;
		}
		int pos = sizeof(metadata);
		fileC.seekg(pos);
		size_t len = 0;
		fileC.read((char*)& len, sizeof(len));
		fileC.read(bitMap, len);
		bitMap[len] = '\0';*/
		return bitMap;
	}

	void setBitMap(char * bm) {
		bitMap = bm;
	}

	void import_1(const char* fileName, char* bloque) {
		fstream fileC(md.nombre, ios::in | ios::out | ios::binary);
		if (!fileC) {
			cout << "Error de apertura en el archivo!" << endl;
			return;
		}

		int indi = findPos(fileName);
			if (indi == -1) { //LO ESCRIBE VACIO. lit solo escribe su nombre en el inode.
				mkdir(fileName, getNodoActual(), false);
				indi = findPos(fileName);
			}

		bloqueDirecto aux;
		
		//vemos en el bitmap cual es el proximo bloque vacio.
		bitMap = getBitMap();
		int indiceBloqueVacio = nextBitAvailable(bitMap);
		indiceBloqueVacio += contNormal;
		contNormal++;
		//nos posicionamos en ese bloque.
		int pos = sizeof(metadata) + md.bitmap_size + (sizeof(inode) * md.cant_entradas) + (sizeof(aux) * (indiceBloqueVacio));
		fileC.seekg(pos);
		fileC.read(reinterpret_cast<char*>(&aux), sizeof(aux));
		aux.bloque = bloque; //SE LE ASIGNA LA INFO DEL IMPORT A NUESTRO VFS.

		//lo volvemos a escribir en el archivo pero corregido.
		pos = sizeof(metadata) + md.bitmap_size + (sizeof(inode) * md.cant_entradas) + (sizeof(aux) * indiceBloqueVacio);
		fileC.seekg(pos);
		fileC.write(reinterpret_cast<char*>(&aux), sizeof(aux));

		fileC.close();
	}

	void export_(const char* fileName) {
		//pasamos de nuestro vfs a la compu.
		fstream fileC(md.nombre, ios::in | ios::out | ios::binary);
		if (!fileC) {
			cout << "Error de apertura en el archivo!" << endl;
			return;
		}

		inode inodo;
		bloqueDirecto aux;

		//saber cuantos bloques uso ese archivo.
		int indiceNodo = findPos(fileName);
		int pos = sizeof(metadata) + md.bitmap_size + (sizeof(inode) * indiceNodo);
		fileC.seekg(pos);
		fileC.read(reinterpret_cast<char*>(&inodo), sizeof(inodo));

		//exportandolo.
		int n = inodo.firstBlock;

		for (size_t i = inodo.firstBlock; i < inodo.cantBloquesUsados+1; i++)
		{
			pos = sizeof(metadata) + md.bitmap_size + (sizeof(inode) * md.cant_entradas) + (sizeof(bloqueDirecto) * i);
			fileC.seekg(pos);
			fileC.read(reinterpret_cast<char*>(&aux), sizeof(aux));
			export_1(fileName, aux);
		}
		
		fileC.close();
	}

	void export_1(const char* fileName, bloqueDirecto aux) {
		string s(fileName);
		//png, jpeg, doc, mp4
		string commands[3] = { ".png", ".jpeg", ".doc"};
		for (size_t i = 0; i < 3; i++)
		{
			int encontro = s.find(commands[i]);
			if (encontro != string::npos) {
				switch (i) {
				case 0:
					s = s.substr(0, s.length() - 4);
					s = s + "1.png";
					break;
				case 1:
					s = s.substr(0, s.length() - 5);
					s = s + "1.jpeg";
					break;
				case 2: 
					s = s.substr(0, s.length() - 4);
					s = s + "1.doc";
					break;
				}
			}
		}

		string s1 = ".mp4";
		int encontro = s.find(s1);
		if (encontro != string::npos) {
			s = s.substr(0, s.length() - 4);
			s = s + "1.mp4";
		}

		ofstream fileC(s, ios::out | ios::app | ios::binary);
		if (!fileC) {
			cout << "Error de apertura en el archivo!" << endl;
			return;
		}
		fileC.seekp(0, ios::end);
		fileC.write(aux.bloque, 4096);
		fileC.close();
	}

	char* updateBitmap(int nBlock, bool on, char * bitMap) {
		//leo el bitmap del archivo
		bitMap = getBitMap();

		if (on)
			bitMap = setOn(bitMap, nBlock);
		else
			bitMap = setOff(bitMap, nBlock);

		/*//escribo el actualizado
		int pos = sizeof(metadata);
		fileC.seekg(pos);
		size_t len = strlen(bitMap);
		fileC.write((char*)& len, sizeof(len));
		fileC.write(bitMap, len);
		fileC.close();*/
		return bitMap;
	}

	
};

