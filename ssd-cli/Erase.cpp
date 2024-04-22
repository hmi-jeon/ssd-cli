#pragma once
#include "ICommand.h"

class Erase : public ICommand {
public:
	virtual string getCommandCode() const override {
		return COMMAND_CODE;
	}

	virtual void execute(vector<string> cmdString, INAND* nand) override {
		if (cmdString.size() != cmdSize) {
			_printInvalidCommand();
			return;
		}

		if (!_isNumber(cmdString[1]) || !_isNumber(cmdString[2])) {
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
			writeBuffer_->data[lba + idx] = "00000000";
			writeBuffer_->dirty[lba + idx] = 1;
		}
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
	static constexpr char COMMAND_CODE[] = "E";

	bool _isValidEraseSize(const int lba, const int size) {
		if (size <= 0 || size > 10) return false;
		if (lba + size > MAX_LBA) return false;

		return true;
	}
};