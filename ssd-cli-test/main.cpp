#include <string>
#include <iostream>

using namespace std;

int main(int argc, char **argv, char **envp) {
	string line;
	while (1) {
		cout << "> ";
		cin >> line;
		if (!line.compare("exit")) {
			cout << "shell exit";
			break;
		}

		cout << line << endl;
	}

	return 0;
}
