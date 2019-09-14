#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <iomanip>
#include "structs.h"

class metodos {

private:
	metadata md;
	int contIndice = 1, contmk = 1, indices = 0;
	inode nodoActual;
	string fecha = "";
	char* bitMap;
	int DB_AMOUNT = 1;

public:

	void initBitMap(char* bitMap, int inodeEntries)
	{
		DB_AMOUNT = 33308 * inodeEntries;
		bitMap = new char[DB_AMOUNT / 8];
		for (int i = 0; i < DB_AMOUNT / 8; i++)
		{
			bitMap[i] = 0;
		}
	}

	void printBitMap(char* bitMap)
	{
		const int SHIFT = 8 * sizeof(char) - 1;
		const char MASK = 1 << SHIFT;

		for (int i = 0; i < DB_AMOUNT / 8; i++)
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

	inode getRaiz() {
		inode temp;
		fstream fileC(md.nombre, ios::in | ios::out | ios::binary);
		if (!fileC) {
			cout << "Error de apertura en el archivo!" << endl;
		}
		int pos = sizeof(metadata);
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

	void createDisc(const char* vfs_name, int inodeEntries) {
		ofstream fileC(vfs_name, ios::out | ios::app | ios::binary);
		if (!fileC) {
			cout << "Error de apertura en el archivo. " << endl;
			return;
		}

		inode inodo;
		if (fecha == "")
			fecha = getDate();

		strcpy_s(md.nombre, vfs_name);
		strcpy_s(md.date, fecha.c_str());
		strcpy_s(md.owner, "Sarah");
		md.cant_entradas = inodeEntries;
		md.tamano = sizeof(md);
		//PONER BITMAP SIZE.
		fileC.write(reinterpret_cast<const char*>(&md), sizeof(md));

		//hace el espacio de los inode entries y los escribe vacios en el archivo. 
		inodo.tamano = sizeof(inodo);
		for (size_t i = 0; i < md.cant_entradas; i++) {
			inodo.indice = indices;
			fileC.write(reinterpret_cast<const char*>(&inodo), sizeof(inodo));
			indices++;
		}
		fileC.close();
		insertRaiz();
	}

	void readFile() {
		ifstream fileC(md.nombre, ios::in | ios::binary);
		if (!fileC) {
			cout << "Error de aprtura en el archivo!" << endl;
		}

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

		int pos = fileC.tellg();
		cout << "byte: " << pos << endl;
		//reading inode entries.
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
		int pos = sizeof(metadata);//+ (sizeof(inode) * nodoActual.indice)
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

		int pos = sizeof(metadata);
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
				pos = sizeof(metadata) + ((sizeof(inode)) * nodoActual.indice);
				fileC.seekg(pos);
				fileC.read(reinterpret_cast<char*>(&temp), sizeof(temp));
				temp.primerHijo = inodeE.indice;
				fileC.seekg(pos);
				fileC.write(reinterpret_cast<char*>(&temp), sizeof(temp));
				setNodoActual(temp);
			}
			else {
				pos = sizeof(metadata) + ((sizeof(inode)) * nodoActual.primerHijo);
				fileC.seekg(pos);
				fileC.read(reinterpret_cast<char*>(&temp), sizeof(temp));
				temp.rightBrother = inodeE.indice;

				fileC.seekg(pos);
				fileC.write(reinterpret_cast<char*>(&temp), sizeof(temp));

			}

			//lo actualiza en el archivo
			pos = sizeof(metadata) + ((sizeof(inode)) * inodeE.indice);
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

		int pos = sizeof(metadata);
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

		int pos = sizeof(metadata) + (sizeof(inode) * nodoActual.indice);
		fileC.seekg(pos);
		fileC.read(reinterpret_cast<char*>(&aux), sizeof(aux));

		bool puede = true;
		pos = sizeof(metadata) + (sizeof(inode) * nodoActual.primerHijo);
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

				pos = sizeof(metadata) + (sizeof(inode) * aux.primerHijo);
				fileC.seekg(pos);
				fileC.read(reinterpret_cast<char*>(&aux), sizeof(aux));
				cout << left << setw(10) << aux.nombre << setw(13) << aux.type
				<< setw(7) << right << aux.tamano << endl;

				//ver si hay right brothers. 
				while (aux.rightBrother != -1) {
					pos = sizeof(metadata) + (sizeof(inode) * aux.rightBrother);
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

		int pos = sizeof(metadata);
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

		int pos = sizeof(metadata) + ((sizeof(inode)) * indice);
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

		int pos = sizeof(metadata) + ((sizeof(inode)) * indice);
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

		int pos = sizeof(metadata) + ((sizeof(inode)) * nodoActual.indice);
		fileC.seekg(pos);
		fileC.read(reinterpret_cast<char*>(&temp), sizeof(temp));

		pos = sizeof(metadata) + (sizeof(inode) * temp.primerHijo);
		fileC.seekg(pos);
		fileC.read(reinterpret_cast<char*>(&temp), sizeof(temp));
		
		return temp.occupied;
	}

	void rm(const char* fileName, int indice) {
		int borrar = findPos(fileName);
		cout << "indicee: " << borrar << endl;
		if (borrar != -1 || (strcmp(fileName, "null") == 0)) {
			cout << "hahaha que pedosss... " << endl;
			inode temp;
			fstream fileC(md.nombre, ios::in | ios::out | ios::binary);
			if (!fileC) {
				cout << "Error de apertura en el archivo!" << endl;
			}

			if (strcmp(fileName, "null") != 0)
				indice = findPos(fileName);

			int pos = sizeof(metadata) + ((sizeof(inode)) * indice);
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
					cout << "temp es: " << temp.nombre << " temp.primerHijo tiene rightB?" << iRBdelHijo << endl;
					if (iRBdelHijo != -1)
						rm("null", iRBdelHijo);
				}
				else {//si tiene hijo pero no rightBrother
					cout << "TIENE HIJO PERO NO RBRO Y ES: " << temp.nombre << endl;
					rm("null", temp.primerHijo);
				}

			}
			else {
				cout << "entra al else, temp es: " << temp.nombre << endl;
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
		
	}
};