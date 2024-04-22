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

		int lba = stoi(cmdString[1]);
		string readData;
		if (writeBuffer_->dirty[lba] == 0) {
			readData = nand->read(lba);
		}
		else {
			readData = writeBuffer_->data[lba];
		}
		_writeResult(readData);
	}

private:
	static constexpr char RESULT_FILE_NAME[] = "result.txt";
	static constexpr int cmdSize = 2;
	static constexpr char COMMAND_CODE[] = "R";

	void _writeResult(string readData) {
		fstream fs;
		fs.open(RESULT_FILE_NAME, ios_base::out);
		fs.write(("0x" + readData).c_str(), readData.size() + 2);
		fs.close();
	}
};