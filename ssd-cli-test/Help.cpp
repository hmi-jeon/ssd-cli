#pragma once
#include "ICommand.cpp"
#include <vector>
#include <iostream>

class Help : public ICommand {
public:
	Help(vector<string> args) {
		this->name = "HELP";
		this->args = args;
	};

	virtual bool execute() override {
		if (!checkValidArguments())
			return false;

		for (int i = 0; i < helps.size(); i++) {
			cout << helps[i][0] << " : " << helps[i][1] << endl;
		}

		return true;
	}

private:
	virtual bool checkValidArguments() override {
		return (args.size() == 1);
	}

	vector<vector<string>> helps = {
		{ "READ","Outputs data written to the LBA address value of the device. (ex. READ 3)" },
		{ "WRITE","Records input data into the designated LBA of the device. (ex. WRITE 1 0x1234ABCD)" },
		{ "EXIT","Exit SHELL. (ex. EXIT)" },
		{ "HELP", "Prints SHELL's command list and help. (ex. HELP)" },
		{ "FULLREAD", "Reads the entire LBA of the device and outputs all data. (ex. FULLREAD)" },
		{ "FULLWRITE", "The input data is recorded in the entire LBA of the device. (ex. FULLWRITE 0x1234ABCD)" },
		{ "TESTAPP1","Test the read and write functions of the entire LBA of the device. (ex. TESTAPP1)" },
		{ "TESTAPP2","Test your device's ability to overwrite data. (ex. TESTAPP2)" }
	};
};