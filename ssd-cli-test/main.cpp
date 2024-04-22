#include "TestShell.cpp"
#include "ICommand.cpp"
#include "Read.cpp"
#include "Write.cpp"
#include "Exit.cpp"
#include "Help.cpp"
#include "FullRead.cpp"
#include "FullWrite.cpp"
#include "TestApp.cpp"
#include "Flush.cpp"
#include "Erase.cpp"
#include "EraseRange.cpp"

using namespace std;

int main(int argc, char **argv, char **envp) {
	vector<ICommand*> commands;
	commands.push_back(new TestApp());
	commands.push_back(new Read());
	commands.push_back(new Write());
	commands.push_back(new Exit());
	commands.push_back(new Help());
	commands.push_back(new FullRead());
	commands.push_back(new FullWrite());
	commands.push_back(new Erase());
	commands.push_back(new Flush());
	commands.push_back(new EraseRange());

	TestShell ssdTestShell(commands);
	ssdTestShell.prompt(argc, argv);
	
	return 0;
}
