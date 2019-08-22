#include <iostream>
#include "structs.h"
#include <string>
using namespace std;

int main(){
	int resp = -1;
	structs s;
	do {
		cout << "Bienvenido al File System 0.2" << endl
			<< "1. Abrir cmd" << endl
			<< "2. Salir" << endl;
		cin >> resp;

		if (resp == 1) {
			//leer lo que el usuario ingresa para ver que accion quiere tomar.
			string command;
			char name[30];
			int cant_entradas = -1;
			cout << "C:\\Users\\Sarah>" << endl;
			cin >> name;
			cout << "Entradas: " << endl;
			cin >> cant_entradas;
			//getline(cin, command);
			
			
			//if ((getCommand(command)) == 0) {
				//name = (command.substr(13, command.length)).c_str();
				s.createFile(name,cant_entradas);
			//}
				s.readFile();

		}

	} while (resp != 2);
}

/*int getCommand(string command) {
	string commands[7] = { "create disc", "mkdir", "cd", "ls", "rm", "Import", "Export" };
	string temp, temp2;
	
	if (command.length > 11)
		temp = command.substr(0, 11);

	if ((temp.compare(commands[0])) == 0) {
		return 0;
	}

	return -1;
}*/