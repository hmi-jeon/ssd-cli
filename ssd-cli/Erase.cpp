#pragma once
#include "ICommand.h"

class Erase : public ICommand {
public:
	virtual string getCommandCode() const override {
		return COMMAND_CODE;
	}

	virtual void execute(vector<string> cmdString, INAND* nand, WriteBuffer& buffer) override {
		if (cmdString.size() != cmdSize) {
			_printInvalidCommand();
			return;
		}

		if (!isNumber(cmdString[1]) || !isNumber(cmdString[2])) {
			_printInvalidCommand();
			return;
		}

		int lba = stoi(cmdString[1]);
		int size = stoi(cmdString[2]);

		if (!_isValidLba(lba) || !_isValidEraseSize(lba, size)) {
			_printInvalidCommand();
			return;
		}

		for (int idx = 0; idx < size; ++idx) {
			buffer.data[lba + idx] = "00000000";
			buffer.dirty[lba + idx] = 1;
		}
		buffer.cnt++;

		if (buffer.cnt >= 10) {
			for (int idx = 0; idx < MAX_LBA; idx++) {
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
	static constexpr char COMMAND_CODE[] = "E";

	bool _isValidEraseSize(const int lba, const int size) {
		if (size <= 0 || size > 10) return false;
		if (lba + size > MAX_LBA) return false;

		return true;
	}
};