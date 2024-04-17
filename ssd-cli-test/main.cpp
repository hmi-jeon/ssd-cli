#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <iostream>
#include "TestShell.cpp"

using namespace std;

int main(int argc, char **argv, char **envp) {
	ssdAPI ssdApi;
	TestShell ssdTestShell(&ssdApi);
	
	string userInput;

	// Sample Main Code
	while (ssdTestShell.getStatus()) {
		cin >> userInput;
		ssdTestShell.inputCommand(userInput);
		cout << userInput << endl;
	}
	
	return 0;
}
