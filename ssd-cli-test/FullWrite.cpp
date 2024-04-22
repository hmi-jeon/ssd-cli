#pragma once
#include "Write.cpp"

class FullWrite : public Write {
public:
	FullWrite() {
		this->name = "FULLWRITE";
		this->description = "The input data is recorded in the entire LBA of the device. (ex. FULLWRITE 0x1234ABCD)";
	};

	virtual bool execute(vector<string> args) override {
		if (!checkValidArguments(args))
			return false;

		for (int lba = 0; lba < MAX_LBA; lba++) {
			_doWrite(lba, args[1]);
		}

		return true;
	}
private:
	virtual bool checkValidArguments(vector<string> args) override {
		if (args.size() != 2) return false;

		return _isValidValue(args[1]);
	}
};
