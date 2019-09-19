#include <iostream>
//#include "structs.h"
#include "metodos.h"
#include <string>
using namespace std;

int main(){
	metodos m;
	string command;
	string init = "";
	init = "\nC:\\Users\\Sarah>";
	const char* vfs_name;
	int n = -1;
	inode na;
	bool vacio = true;
	na = m.getNodoActual();
	
	cout << "Microsoft Windows [Version 10.0.18362.356] \n(c)2019 Microsoft Corporation.All rights reserved.\n " << endl;

	while (true) {
		cout << init;
		getline(cin, command);

		string commands[7] = { "create disc", "mkdir", "cd", "ls", "rm", "Import", "Export" };

		bool found = false;
		for (int i = 0; i < 7; i++)
		{
			int encontro = command.find(commands[i]);
			if (encontro != string::npos && encontro == 0) {
				na = m.getNodoActual();
				found = true;
				switch (i) {
				case 0:
					if (command.length() >= 12) {//CREATE DISC
						if (vacio) {
							command = command.substr(12, command.length());
							vfs_name = command.c_str();
							cout << "# de inode entries: " << endl;
							cin >> n;
							m.createDisc(vfs_name, n + 1);
							vacio = false;
						}
						//m.readFile();
					}
					break;
				case 1:
					if (command.length() >= 6) {//MKDIR
						command = command.substr(6, command.length());
						vfs_name = command.c_str();
						m.mkdir(vfs_name,na, true);
					}
					break;
				case 2:
					if (command.length() >= 3) {//CD
						int occu = m.occupied();
						if (occu == 1) {
							command = command.substr(3, command.length());
							vfs_name = command.c_str();
							int encontro1 = command.find("..");
							if (encontro1 != string::npos && encontro1 == 0) {
								m.cd(vfs_name, true);
								init = init.substr(0, 16);
								na = m.getNodoActual();
								if (na.indice != 0)
									init = init + na.nombre + ">";
							}
							else {
								m.cd(vfs_name, false);
								na = m.getNodoActual();
								init = init + na.nombre + ">";
							}
							//cout << "currently in: " << na.nombre;	
						}else
							cout << "Disco vacio." << endl;
					}	
					break;
				case 3: 
					if (command.length() == 2) {//LS
						na = m.getNodoActual();
						m.ls(na);
					}
					break;
				case 4: 
					if (command.length() >= 3) {//RM
						na = m.getNodoActual();
						int find = command.find(na.nombre);
						command = command.substr(3, command.length());
						vfs_name = command.c_str();
						bool itsDot = m.rm(vfs_name,-1);
						if (itsDot) {
							init = init.substr(0, (init.length() - (command.length())-1));
						}
						else {
							init = init.substr(0, 16);
							na = m.getNodoActual();
							if (na.indice != 0)
							init = init + ">" + na.nombre;
						}
					}
					break;
				case 5:
					if (command.length() >= 7) {//IMPORT
						command = command.substr(7, command.length());
						vfs_name = command.c_str();
						m.import(vfs_name);
					}
					break;
				case 6:
					if (command.length() >= 7) {//EXPORT
						command = command.substr(7, command.length());
						vfs_name = command.c_str();
						m.export_(vfs_name);
					}
					break;
				default:
					cout << "VFS name not found. " << endl;
				}
			}
			if (!command.empty() && !found && i == 6)
				cout << "'" << command << "'" << "is not recognized as an internal or external command, operable program or batch file." << endl;
		}
	}
		
}

