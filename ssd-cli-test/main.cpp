#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <iostream>
#include "TestShell.cpp"

using namespace std;

int main(int argc, char **argv, char **envp) {
	TestShell ssdTestShell;
	int a = 0;
	// Sample Main Code
	while (ssdTestShell.status) {
		cout << "> ";
		cin >> ssdTestShell.line;
		if (!ssdTestShell.line.compare("exit"))
			ssdTestShell.status = false;


		cout << ssdTestShell.line << endl;
	}

	return 0;
}
