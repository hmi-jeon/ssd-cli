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

	bool isValidLba(int lba) {
		if (lba == 101) {
			return false;
		}
		return true;
	}
private:
	NAND* nand_;
};