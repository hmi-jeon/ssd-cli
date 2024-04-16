#include "ssd.cpp"


int main(int argc, char* argv[]) {
	NAND NAND;
	SSD ssd(&NAND);
	ssd.command(argc, argv);

	return 0;
}