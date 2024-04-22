#include "ssd.cpp"
#include "virtual_nand.cpp"
#include "ICommand.h"
#include "Read.cpp"
#include "Write.cpp"
#include "Erase.cpp"
#include "Flush.cpp"

WriteBuffer* ICommand::writeBuffer_ = nullptr;

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