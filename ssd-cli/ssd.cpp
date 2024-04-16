#include <iostream>
#include <string>
#include <vector>
#include "nand.cpp"

using namespace std;

class SSD {
public:
	SSD(lNAND* nand)
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

	void read(int lba) {
		if (!isValidLba(lba)) {
			std::cout << "Invalid Parameter" << std::endl;
			return;
		}
			
		nand_->read(lba);
	}

	void write(int lba, string value) {
		if (!isValidLba(lba) || !isValidValue(value)) {
			std::cout << "Invalid Parameter" << std::endl;
			return;
		}

		nand_->write(lba, value.substr(2));
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
			if (!isxdigit(c)) {
				return false;
			}
		}
		return true;
	}
private:
	lNAND* nand_;
};