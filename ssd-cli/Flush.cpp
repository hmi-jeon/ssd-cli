#pragma once
#include "ICommand.h"

class Flush : public ICommand {
public:
	virtual string getCommandCode() const override {
		return COMMAND_CODE;
	}

	virtual void execute(vector<string> cmdString, INAND* nand) override {
		if (cmdString.size() != cmdSize) {
			_printInvalidCommand();
			return;
		}

		for (int idx = 0; idx < MAX_LBA; idx++) {
			if (writeBuffer_->dirty[idx] == 1) {
				nand->write(idx, writeBuffer_->data[idx]);
			}
			writeBuffer_->dirty[idx] = 0;
		}
		writeBuffer_->cnt = 0;
	}

private:
	static constexpr int cmdSize = 1;
	static constexpr char COMMAND_CODE[] = "F";
};