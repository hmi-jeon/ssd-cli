#include "ssd.cpp"
#include "virtual_nand.cpp"


int main(int argc, char* argv[]) {
	VirtualNAND vnand;
	SSD ssd(&vnand);
	ssd.command(argc, argv);

	return 0;
}