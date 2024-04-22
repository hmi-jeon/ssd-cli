#include <string>
#include <iostream>
#include "TestShell.cpp"

using namespace std;

int main(int argc, char* argv[]) {

	TestShell testShell;
	testShell.selectExecuteMode(argc, argv);

	return 0;
}
