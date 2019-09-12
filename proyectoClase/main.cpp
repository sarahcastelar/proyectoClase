#include <iostream>
#include "structs.h"
#include <string>
using namespace std;

/*const char* getName(string name) {
	char const* vfs_name;
	for (int i = 0; i < name.length() - 1; i++) {
		vfs_name += name[i];
	}
	return vfs_name;
}*/

int main(){
	structs s;
	string command;
	const char* vfs_name;
	int n = -1;

	cout << "Microsoft Windows [Version 10.0.18362.356] \n(c)2019 Microsoft Corporation.All rights reserved.\n " << endl;

	while (true) {
		cout << "\nC:\\Users\\Sarah>";
		getline(cin, command);

		string commands[7] = { "create disc", "mkdir", "cd", "ls", "rm", "Import", "Export" };
		//first mkdir, cd, ls, rm, create disc, import, export

		bool found = false;
		for (int i = 0; i < 7; i++)
		{
			int encontro = command.find(commands[i]);
			if (encontro != string::npos && encontro == 0) {
				found = true;
				switch (i) {
				case 0:
					command = command.substr(12, command.length());
					vfs_name = command.c_str();
					//s.createDisc(vfs_name);
					break;
				case 1:
					command = command.substr(5, command.length());
					vfs_name = command.c_str();
					//s.createDisc(vfs_name);
					break;
				case 2:
					command = command.substr(3, command.length());
					vfs_name = command.c_str();
					//s.createDisc(vfs_name);
					s.getDate();
					break;
				case 3: 
					command = command.substr(3, command.length());
					vfs_name = command.c_str();
					//s.createDisc(vfs_name);
					break;
				case 4: 
					command = command.substr(3, command.length());
					vfs_name = command.c_str();
					//s.createDisc(vfs_name);
					break;
				case 5:
					command = command.substr(6, command.length());
					vfs_name = command.c_str();
					//s.createDisc(vfs_name);
					break;
				case 6:
					command = command.substr(6, command.length());
					vfs_name = command.c_str();
					//s.createDisc(vfs_name);
					break;
				}
			}
			if (!command.empty() && !found && i == 6)
				cout << "'" << command << "'" << "is not recognized as an internal or external command, operable program or batch file." << endl;
		}


		/*if (resp == 1) {
			//leer lo que el usuario ingresa para ver que accion quiere tomar.

			string nameS;
			char const* name;
			int cant_entradas = -1;

			cin >> command;


			string temp, temp2;

			temp = command.substr(0, 10);


			if (command.length > 10) {
				temp = command.substr(0, 10);
				if ((temp.compare(commands[0])) == 0) {
					nameS = command.substr(13, command.length());
					name = getName(nameS);
					s.createDisc(name);
				}
				else if () {

				}
				else
					cout << "'" << command << "'"
					<< "is not recognized as an internal or external command, operable program or batch file." << endl;
			}

			//cin >> name;
			cout << "Entradas: " << endl;
			cin >> cant_entradas;
			//getline(cin, command);


			//if ((getCommand(command)) == 0) {
				//name = (command.substr(13, command.length)).c_str();
				s.createFile(name,cant_entradas);
			//}
				s.readFile();

		}*/
	}
		
}

