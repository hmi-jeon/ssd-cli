#include "TestShell.cpp"

using namespace std;

int main(int argc, char **argv, char **envp) {

	TestShell ssdTestShell;
	ssdTestShell.prompt(argc, argv);
	
	return 0;
}
