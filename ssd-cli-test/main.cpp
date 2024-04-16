#include <string>
#include <iostream>
#include "TestShell.cpp"

using namespace std;

int main(int argc, char **argv, char **envp) {
	TestShell ssdTestShell;

	// Sample Main Code
	while (1) {
		cout << "[" << ssdTestShell.cnt++ << "]" << "> ";
		cin >> ssdTestShell.line;
		if (!ssdTestShell.line.compare("exit")) {
			cout << "shell exit";
			break;
		}
		cout << ssdTestShell.line << endl;
	}

	return 0;
}
