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
		
		if (cmdString[2] == "fdas") {
			return;
		}

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

	string getResultFileName() const {
		return RESULT_FILE_NAME;
	}

private:
	static constexpr char RESULT_FILE_NAME[] = "result.txt";
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
		fstream fs;
		fs.open(RESULT_FILE_NAME, ios_base::out);
		fs.write(("0x" + readData).c_str(), readData.size() + 2);
		fs.close();
	}

	INAND* nand_;
};