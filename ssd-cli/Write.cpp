#pragma once
#include "ICommand.h"

class Write : public ICommand {
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

		if (!_isValidLba(stoi(cmdString[1])) || !_isValidValue(cmdString[2])) {
			_printInvalidCommand();
			return;
		}

		int lba = stoi(cmdString[1]);
		string writeData = cmdString[2].substr(2);

		writeBuffer_->data[lba] = writeData;
		writeBuffer_->dirty[lba] = 1;
		writeBuffer_->cnt++;

		if (writeBuffer_->cnt >= 10) {
			for (int idx = 0; idx < MAX_LBA; idx++) {
				if (writeBuffer_->dirty[idx] == 1) {
					nand->write(idx, writeBuffer_->data[idx]);
				}
				writeBuffer_->dirty[idx] = 0;
			}
			writeBuffer_->cnt = 0;
		}
	}

private:
	static constexpr int cmdSize = 3;
	static constexpr char COMMAND_CODE[] = "W";
};
