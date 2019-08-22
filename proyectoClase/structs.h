#include <iostream>
#include <fstream>
using namespace std;

struct metadata {
	char nombre[30];
	unsigned int tamano;
	char date[8];
	char owner[30];
	unsigned int cant_entradas;
	unsigned int bitmap_size;
	//unsigned int cant_bloques_directos;
	metadata() {
		cant_entradas = -1;
		bitmap_size = -1;
	}
}; 

struct inode {
	char nombre[30];
	char type;
	char fecha_creacion[8];
	int tamano;
	int padre;
	int primerHijo;
	int rightChild;
};


class structs {
private:
	metadata md;


public:
	void createMetadata(const char* vfs_name) {
		unsigned int tamano;
		char date[8];
		char owner[30];
		unsigned int cant_entradas;
		unsigned int bitmap_size;
		unsigned int cant_bloques_directos;

		strcpy_s(md.nombre, vfs_name);
		cout << "Ingrese fecha: " << endl;
		cin >> date;
		strcpy_s(md.date, date);
		cout << "Ingrese autor: " << endl;
		cin >> owner;
		strcpy_s(md.owner, owner);
	}

	void readMetaData() {
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
		fileC.close();
	}

	void createBitMap() {

	}

	void readBitMap() {

	}

	void createFile(const char* vfs_name) {
		createMetadata(vfs_name);
		md.tamano = sizeof(md);

		ofstream fileC(vfs_name, ios::out | ios::app | ios::binary);
		if (!fileC) {
			cout << "Error de apertura en el archivo. " << endl;
			return;
		}

		fileC.seekp(0, ios::end);
		fileC.write(reinterpret_cast<const char*>(&md), sizeof(md)); //se escribe el metadata. 
		//se escribe el bitmap

		//se hace espacio para los directorios.
		fileC.close();
	}
};
