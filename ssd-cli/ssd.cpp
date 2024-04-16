#include <string>
#include "nand.cpp"

using namespace std;

class SSD {
public:
	SSD(NAND* nand)
		: nand_(nand) {

	}

	void read(int lba) {
		nand_->read(lba);
	}

	void write(int lba, string value) {
		nand_->write(lba, value);
	}

private:
	NAND* nand_;
};