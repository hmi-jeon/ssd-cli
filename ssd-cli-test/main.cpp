#include <string>
#include <iostream>
#include "TestShell.cpp"

using namespace std;

int main(int argc, char **argv, char **envp) {

	TestShell ssdTestShell;

	ssdTestShell.Runner();

	return 0;

	if (argc == 2 && argv[1] == RUN_LIST) {
		ssdTestShell.Runner();
	}

	if (argc == 1) {
		char userInput[100];

		while (1) {
			cout << "> ";
			cin.getline(userInput, 100);
			if (userInput[0] == '\0') {
				continue;
			}
			ssdTestShell.inputCommand(userInput);
		}
	}

	return 0;
}
