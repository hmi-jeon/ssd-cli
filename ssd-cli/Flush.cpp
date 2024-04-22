#pragma once
#include "Write.cpp"

class Flush : public Write {
public:
	virtual string getCommandCode() const override {
		return COMMAND_CODE;
	}

	virtual void execute(vector<string> cmdString, INAND* nand) override {
		if (cmdString.size() != cmdSize) {
			_printInvalidCommand();
			return;
		}

		_writeBufferToNand(nand);
	}

private:
	static constexpr int cmdSize = 1;
	static constexpr char COMMAND_CODE[] = "F";
};