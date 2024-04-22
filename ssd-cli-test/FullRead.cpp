#pragma once
#include "ICommand.cpp"
#include <vector>
#include "Read.cpp"


class FullRead : public ICommand {
public:
	FullRead(vector<string> args) {
		this->name = "FULLREAD";
		this->args = args;
	};

	virtual bool execute() override {
		if (!checkValidArguments())
			return false;

		for (int lba = 0; lba < MAX_LBA; lba++) {
			callArgs.clear();
			callArgs.push_back("READ");
			callArgs.push_back(to_string(lba));

			ICommand* com = new Read(callArgs);
			com->execute();
		}

		return true;
	}

private:
	virtual bool checkValidArguments() override {
		return (args.size() == 1);
	}

	vector<string> callArgs;
};
