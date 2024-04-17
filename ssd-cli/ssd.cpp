#include <iostream>
#include <string>
#include <vector>
#include <fstream>
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
		_printInvalidCommand();
	}

	void read(const int lba) {
		if (!_isValidLba(lba)) {
			_printInvalidCommand();
			return;
		}
			
		string readData = nand_->read(lba);
		_writeResult(readData);
	}

	void write(const int lba, const string value) {
		if (!_isValidLba(lba) || !_isValidValue(value)) {
			_printInvalidCommand();
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

	void _printInvalidCommand() {
		std::cout << "INVALID COMMAND" << std::endl;
	}
	
	void _writeResult(string readData) {
		const string fileName = "result.txt";
		fstream fs;
		fs.open(fileName.c_str(), ios_base::out);
		fs.write(("0x" + readData).c_str(), readData.size() + 2);
		fs.close();
	}

	INAND* nand_;
};