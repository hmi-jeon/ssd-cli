#pragma once
#include "Read.cpp"


class FullRead : public Read {
public:
	FullRead() {
		this->name = "FULLREAD";
		this->description = "Reads the entire LBA of the device and outputs all data. (ex. FULLREAD)";
	};

	virtual bool execute(vector<string> args) override {
		if (!checkValidArguments(args))
			return false;

		for (int lba = 0; lba < MAX_LBA; lba++) {
			_doRead(lba);
			_printResult();
		}

		return true;
	}

private:
	virtual bool checkValidArguments(vector<string> args) override {
		return (args.size() == 1);
	}
};
