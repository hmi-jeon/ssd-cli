#pragma once
#include "ICommand.h"

class Read : public ICommand {
public:
	virtual string getCommandCode() const override {
		return COMMAND_CODE;
	}

	virtual void execute(vector<string> cmdString, INAND* nand) override {
		if (cmdString.size() != cmdSize) {
			_printInvalidCommand();
			return;
		}

		if (!isNumber(cmdString[1])) {
			_printInvalidCommand();
			return;
		}

		if (!_isValidLba(stoi(cmdString[1]))) {
			_printInvalidCommand();
			return;
		}

		_writeResult(_readData(nand, stoi(cmdString[1])));
	}

private:
	static constexpr char RESULT_FILE_NAME[] = "result.txt";
	static constexpr int cmdSize = 2;
	static constexpr char COMMAND_CODE[] = "R";

	string _readData(INAND* nand, int lba) {
		if (writeBuffer_->dirty[lba] == 0) {
			return nand->read(lba);
		}
		else {
			return writeBuffer_->data[lba];
		}
	}

	void _writeResult(string readData) {
		fstream fs;
		fs.open(RESULT_FILE_NAME, ios_base::out);
		fs.write(("0x" + readData).c_str(), readData.size() + 2);
		fs.close();
	}
};