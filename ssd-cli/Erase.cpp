#pragma once
#include "Command.cpp"

class Erase : public Command {
public:
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
	static constexpr int MAX_LBA = 100;

	bool _isValidEraseSize(const int lba, const int size) {
		if (size <= 0 || size > 10) return false;
		if (lba + size > MAX_LBA) return false;

		return true;
	}
};