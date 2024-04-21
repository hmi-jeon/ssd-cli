#include "ssd.cpp"
#include "virtual_nand.cpp"
#include "ICommand.h"
#include "Read.h"
#include "Write.h"
#include "Erase.h"
#include "Flush.h"

int main(int argc, char* argv[]) {
	VirtualNAND nand;
	SSD ssd(&nand);

	vector<ICommand*> commands;
	commands.push_back(new Read());
	commands.push_back(new Write());
	commands.push_back(new Erase());
	commands.push_back(new Flush());
	ssd.init(commands);
	ssd.command(argc, argv);

	return 0;
}