#pragma once
#include "Command.cpp"

class Flush : public Command {
public:
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
};