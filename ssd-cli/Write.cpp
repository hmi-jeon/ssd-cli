#pragma once
#include "Command.cpp"

class Write : public Command {
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

		if (!_isValidLba(stoi(cmdString[1])) || !_isValidValue(cmdString[2])) {
			_printInvalidCommand();
			return;
		}

		int lba = stoi(cmdString[1]);
		string writeData = cmdString[2].substr(2);

		buffer.data[lba] = writeData;
		buffer.dirty[lba] = 1;
		buffer.cnt++;

		if (buffer.cnt >= 10) {
			for (int idx = 0; idx < 100; idx++) {
				if (buffer.dirty[idx] == 1) {
					nand->write(idx, buffer.data[idx]);
				}
				buffer.dirty[idx] = 0;
			}
			buffer.cnt = 0;
		}
	}

private:
	static constexpr int cmdSize = 3;
};
