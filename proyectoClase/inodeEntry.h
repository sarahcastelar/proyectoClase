#include <iostream> 
using namespace std;

struct inode {
	char nombre[30];
	char type;
	char fecha_creacion[8];
	int tamano;
	int padre;
	int primerHijo;
	int rightChild;
};

class inodeEntry {

private:
	

public:

	void createInode() {
		char nombre[30];
		char type;
		char fecha_creacion[8];
		int tamano;
		int padre;
		int primerHijo;
		int rightChild;

		cout << "Ingrese nombre: " << endl;
		cin >> nombre; 
		cout << "Ingrese tipo: " << endl;
		cin >> type;
		cout << "Ingrese fecha: " << endl;
		cin >> fecha_creacion;
		
		//el padre, hijo y hermanos lo hace segun del directorio de donde este. 

	}

};