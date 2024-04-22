#pragma once
#include "ICommand.cpp"
#include <vector>
#include "Read.cpp"


class FullRead : public ICommand {
public:
	FullRead() {
		this->name = "FULLREAD";
	};

	virtual bool execute(vector<string> args) override {
		if (!checkValidArguments(args))
			return false;

		for (int lba = 0; lba < MAX_LBA; lba++) {
			callArgs.clear();
			callArgs.push_back("READ");
			callArgs.push_back(to_string(lba));

			ICommand* com = new Read();
			com->execute(callArgs);
		}

		return true;
	}

private:
	virtual bool checkValidArguments(vector<string> args) override {
		return (args.size() == 1);
	}

	vector<string> callArgs;
};
