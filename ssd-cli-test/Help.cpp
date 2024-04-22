#pragma once
#include "ICommand.cpp"
#include <vector>
#include <iostream>

class Help : public ICommand {
public:
	Help() {
		this->name = "HELP";
	};

	virtual bool execute(vector<string> args) override {
		if (!checkValidArguments(args))
			return false;

		for (int i = 0; i < helps.size(); i++) {
			cout << helps[i][0] << " : " << helps[i][1] << endl;
		}

		return true;
	}

private:
	virtual bool checkValidArguments(vector<string> args) override {
		return (args.size() == 1);
	}

	vector<vector<string>> helps = {
		{ "READ","Outputs data written to the LBA address value of the device. (ex. READ 3)" },
		{ "WRITE","Records input data into the designated LBA of the device. (ex. WRITE 1 0x1234ABCD)" },
		{ "EXIT","Exit SHELL. (ex. EXIT)" },
		{ "HELP", "Prints SHELL's command list and help. (ex. HELP)" },
		{ "FULLREAD", "Reads the entire LBA of the device and outputs all data. (ex. FULLREAD)" },
		{ "FULLWRITE", "The input data is recorded in the entire LBA of the device. (ex. FULLWRITE 0x1234ABCD)" },
		{ "FLUSH", "The data in the write buffer are applied to the SSD." },
		{ "ERASE", "Delete data from a specific LBA to a specific size. (ex. ERASE 3 5)"},
		{ "ERASE_RANGE", "Delete data from startLBA to endLBA (ex. ERASE 20 25)"}
	};
};