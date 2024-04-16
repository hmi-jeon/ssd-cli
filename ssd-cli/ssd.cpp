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

	bool isValidValue(const string value) {
		if (value.size() != 10)
			return false;

		if (value.substr(0, 2) != "0x")
			return false;

		for (const char& c : value.substr(2)) {
			if (!isdigit(c)) {
				return false;
			}
		}
		return true;
	}
private:
	NAND* nand_;
};