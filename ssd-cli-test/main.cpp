#include <string>
#include <iostream>
#include "TestShell.cpp"

using namespace std;

int main(int argc, char **argv, char **envp) {

	TestShell ssdTestShell;

	if (argc == 2) {
		string firstArg = argv[1];
		if(firstArg == RUN_LIST) ssdTestShell.Runner();
	}

	if (argc == 1) {
		char userInput[100];

		while (1) {
			cout << "> ";
			cin.getline(userInput, 100);
			if (userInput[0] == '\0') {
				continue;
			}
			ssdTestShell.executeCommand(userInput);
		}
	}

	return 0;
}
