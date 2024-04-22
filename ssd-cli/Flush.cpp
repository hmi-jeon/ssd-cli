#pragma once
#include "ICommand.h"

class Flush : public ICommand {
public:
	virtual string getCommandCode() const override {
		return COMMAND_CODE;
	}

	virtual void execute(vector<string> cmdString, INAND* nand, WriteBuffer& buffer) override {
		if (cmdString.size() != cmdSize) {
			_printInvalidCommand();
			return;
		}

		for (int idx = 0; idx < 100; idx++) {
			if (buffer.dirty[idx] == 1) {
				nand->write(idx, buffer.data[idx]);
			}
			buffer.dirty[idx] = 0;
		}
		buffer.cnt = 0;
	}

private:
	static constexpr int cmdSize = 1;
	static constexpr char COMMAND_CODE[] = "F";
};