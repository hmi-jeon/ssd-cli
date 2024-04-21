#pragma once
#include "Command.cpp"

class Read : public Command {
public:
	virtual void execute(vector<string> cmdString, INAND* nand, WriteBuffer& buffer) override {
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
		if (buffer.dirty[lba] == 0) {
			readData = nand->read(lba);
		}
		else {
			readData = buffer.data[lba];
		}
		_writeResult(readData);
	}

private:
	static constexpr char RESULT_FILE_NAME[] = "result.txt";
	static constexpr int cmdSize = 2;

	void _writeResult(string readData) {
		fstream fs;
		fs.open(RESULT_FILE_NAME, ios_base::out);
		fs.write(("0x" + readData).c_str(), readData.size() + 2);
		fs.close();
	}
};