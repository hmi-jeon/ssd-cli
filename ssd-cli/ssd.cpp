#include <iostream>
#include <string>
#include <vector>
#include "inand.h"

using namespace std;

class SSD {
public:
	SSD(INAND* nand)
		: nand_(nand) {

	}

	void command(int argc, char* argv[])
	{
		std::vector<std::string> cmdString(argv, argv + argc);
		
		if (argc == 4 && cmdString[1]._Equal("W")) {
			write(std::stoi(cmdString[2]), cmdString[3]);
			return;
		}
		if (argc == 3 && cmdString[1]._Equal("R")) {
			read(std::stoi(cmdString[2]));
			return;
		}
		cout << "INVALID COMMAND" << endl;
	}

	void read(const int lba) {
		if (!_isValidLba(lba)) {
			std::cout << "Invalid Parameter" << std::endl;
			return;
		}
			
		nand_->read(lba);
	}

	void write(const int lba, const string value) {
		if (!_isValidLba(lba) || !_isValidValue(value)) {
			std::cout << "Invalid Parameter" << std::endl;
			return;
		}

		nand_->write(lba, value.substr(2));
	}

private:
	bool _isValidLba(const int lba) {
		return (lba >= 0 && lba < 100);
	}

	bool _isValidValue(const string value) {
		if (value.size() != 10)
			return false;

		if (value.substr(0, 2) != "0x")
			return false;

		for (const char& c : value.substr(2)) {
			if (!isxdigit(c)) {
				return false;
			}
		}
		return true;
	}

	INAND* nand_;
};