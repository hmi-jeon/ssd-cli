#include "ssd.cpp"
#include "virtual_nand.cpp"


int main(int argc, char* argv[]) {
	VirtualNAND nand;
	SSD ssd(&nand);
	ssd.command(argc, argv);

	return 0;
}