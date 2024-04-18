#include <string>
#include <iostream>
#include "TestShell.cpp"

using namespace std;

int main(int argc, char **argv, char **envp) {

	TestShell ssdTestShell;
	
	//string userInput;
	char userInput[100];

	// Sample Main Code
	while (1) {
		cout << "> ";
		cin.getline(userInput, 100);
		if (userInput[0] == '\0') {
			continue;
		}
		ssdTestShell.inputCommand(userInput);
	}
	return 0;
}
