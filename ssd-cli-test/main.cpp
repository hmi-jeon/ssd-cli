#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <iostream>
#include "TestShell.cpp"

using namespace std;

int main(int argc, char **argv, char **envp) {
	ssdAPI ssdApi;
	TestShell ssdTestShell(&ssdApi);
	
	//string userInput;
	char userInput[100];

	// Sample Main Code
	while (ssdTestShell.getStatus()) {
		cout << "> ";
		cin.getline(userInput, 100);
		if (userInput[0] == '\0') {
			continue;
		}
		ssdTestShell.inputCommand(userInput);
	}
	
	return 0;
}
