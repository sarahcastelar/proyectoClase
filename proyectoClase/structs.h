#include <iostream>
#include <fstream>
#include<ctime> 
#include <string>
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
		cant_entradas = 0;
		bitmap_size = 0;
	}
}; 

struct inode {
	char nombre[30];
	char type;
	char fecha_creacion[9];
	int tamano;
	int padre;
	int primerHijo;
	int rightBrother;
	//unsigned int bloquesDirectos[12];
	//unsigned int * 
	inode() {
		padre = -1;
		primerHijo = -1;
		rightBrother = -1;
		strcpy_s(nombre, "null");
		tamano = -1;
		type = '-';
		strcpy_s(fecha_creacion, "null");
	}
};


class structs {
private:
	metadata md;
	inode inodeEntry;
	inode* inodeEntries;
	int cant_entradas = 10;

public:

	char* getDate() {
		time_t t = time(NULL);
		tm* timePtr = localtime(&t);
		string aux = "";
		char fechaCreacion[9];

		(timePtr->tm_mday < 10) ? aux = "0" + to_string(timePtr->tm_mday) : aux = to_string(timePtr->tm_mday);
		(timePtr->tm_mon < 10) ? aux = aux + "0" + to_string(timePtr->tm_mon + 1) : aux = aux + to_string(timePtr->tm_mon + 1); //ver que pedoss con el, me imprimia 8 en vez de 9
		aux = aux + to_string(timePtr->tm_year + 1900);
		for (size_t i = 0; i < 8; i++)
		{
			fechaCreacion[i] = aux.at(i);
		}
	}

	inode* getInodeEntries() {
		return inodeEntries;
	}

	void setInodeEntries(inode* inodeEntries) {
		this->inodeEntries = inodeEntries;
	}

	void createMetadata(const char* vfs_name, int cant_entradas) {
		unsigned int bitmap_size;
		unsigned int cant_bloques_directos;

		strcpy_s(md.nombre, vfs_name);
		strcpy_s(md.date, getDate());
		strcpy_s(md.owner, "Sarah");
		//md.cant_entradas = cant_entradas;
	}

	void createBitMap() {

	}

	void readBitMap() {

	}

	inode createInodeEntry(int i) {
		inode temporal;
		/*
			hacer que el inodeEntry quede como en null somehow.
			para que no guarde lo mismo que el anterior.
		*/
		char nombre[30];
		char type;
		char fecha_creacion[9];
		int tamano;
		int padre;
		int primerHijo;
		int rightBrother;

		cout << "Ingrese nombre del Inodo: " << endl;
		cin >> nombre;
		strcpy_s(temporal.nombre, nombre);
		cout << "Ingrese tipo:" << endl;
		cin >> type;
		temporal.type = type;
		cout << "Ingrese fecha: " << endl;
		cin >> fecha_creacion;
		strcpy_s(temporal.fecha_creacion, fecha_creacion);
		inodeEntries[i] = temporal;
		return temporal;
	}

	void createDisc(const char* vfs_name) {
		ofstream fileC(vfs_name, ios::out | ios::app | ios::binary);
		if (!fileC) {
			cout << "Error de apertura en el archivo. " << endl;
			return;
		}
		createMetadata(vfs_name, cant_entradas);
		inodeEntries = new inode[md.cant_entradas]; //GLOBAL.
		md.tamano = sizeof(md);
	}

	void createFile(const char* vfs_name, int cant_entradas){
		ofstream fileC(vfs_name, ios::out | ios::app | ios::binary);
		if (!fileC) {
			cout << "Error de apertura en el archivo. " << endl;
			return;
		}

		createMetadata(vfs_name, cant_entradas);
		inodeEntries = new inode[md.cant_entradas]; //GLOBAL.
		md.tamano = sizeof(md);

		fileC.seekp(0, ios::end);
		fileC.write(reinterpret_cast<const char*>(&md), sizeof(md)); //se escribe el metadata. 
		//Creating inode entries.
		inode inode1;
		for (int i = 0; i < md.cant_entradas; i++){
			
			
			inode1 = createInodeEntry(i);
			fileC.write(reinterpret_cast<const char*>(&inode1), sizeof(inode1));
		}

		fileC.close();
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
		cout << "Nombre del archivo: " << struct_md.nombre << endl
			<< "Fecha creacion: " << struct_md.date << endl
			<< "Autor: " << struct_md.owner << endl
			<< "Tamano: " << struct_md.tamano << endl
			<< "Inode Entries: " << struct_md.cant_entradas << endl
			<< "BitMap size: " << struct_md.bitmap_size << endl;
		cout << "--------fin leyendo meta------" << endl;

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
				<< "Right brother: " << struct_i.rightBrother << endl;

			fileC.read(reinterpret_cast<char*>(&struct_i), sizeof(struct_i));
		}
		cout << "--------fin leyendo------" << endl;
		fileC.close();
	}

};
