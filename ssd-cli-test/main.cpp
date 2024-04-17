#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <iostream>
#include "TestShell.cpp"

using namespace std;

int main(int argc, char **argv, char **envp) {
	ssdAPI ssdApi;
	TestShell ssdTestShell(&ssdApi);
	int a = 0;
	// Sample Main Code
	while (ssdTestShell.getExit()) {
		ssdTestShell.read(a++);
		cout << "> ";
		cin >> ssdTestShell.line;
		cout << ssdTestShell.line << endl;
	}

	return 0;
}
