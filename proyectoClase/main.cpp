#include <iostream>
//#include "structs.h"
#include "metodos.h"
#include <string>
using namespace std;

int main(){
	metodos m;
	string command;
	const char* vfs_name;
	int n = -1;
	inode na;
	na = m.getNodoActual();
	
	cout << "Microsoft Windows [Version 10.0.18362.356] \n(c)2019 Microsoft Corporation.All rights reserved.\n " << endl;

	while (true) {
		cout << "\nC:\\Users\\Sarah>";
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
					if (command.length() >= 12) {
						command = command.substr(12, command.length());
						vfs_name = command.c_str();
						cout << "# de inode entries: " << endl;
						cin >> n;
						m.createDisc(vfs_name, n+1);\
						m.insertRaiz();
						//m.readFile();
					}
					break;
				case 1:
					if (command.length() >= 6) {
						command = command.substr(6, command.length());
						vfs_name = command.c_str();
						m.mkdir(vfs_name,na);
					}
					break;
				case 2:
					if (command.length() >= 3) {
						int encontro1 = command.find("/..");
						if (encontro1 != string::npos && encontro1 == 3) {
							cout << "sarah acordate de hacer esto. " << endl;
						}
						else {
							command = command.substr(3, command.length());
							vfs_name = command.c_str();
							cout << "vfsname:" << vfs_name << endl;
							m.cd(vfs_name);
						}
						na = m.getNodoActual();
						cout << "currently in: " << na.nombre;
					}
					break;
				case 3: 
					if (command.length() >= 3) {
						command = command.substr(3, command.length());
						vfs_name = command.c_str();
						//s.createDisc(vfs_name);
					}
					break;
				case 4: 
					if (command.length() >= 3) {
						command = command.substr(3, command.length());
						vfs_name = command.c_str();
						//s.createDisc(vfs_name);
					}
					break;
				case 5:
					if (command.length() >= 7) {
						command = command.substr(7, command.length());
						vfs_name = command.c_str();
						//s.createDisc(vfs_name);
					}
					break;
				case 6:
					if (command.length() >= 7) {
						command = command.substr(7, command.length());
						vfs_name = command.c_str();
						//s.createDisc(vfs_name);
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

