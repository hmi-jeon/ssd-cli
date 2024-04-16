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
		return (lba >= 0 && lba < 100);
	}

	bool isValidValue(string value) {
		if (value == "a") {
			return false;
		}
		return true;
	}
private:
	NAND* nand_;
};