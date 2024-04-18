#pragma once
#include "ICommand.cpp"
#include <vector>
#include "Write.cpp"

class FullWrite : public ICommand {
public:
	FullWrite(vector<string> args) {
		this->name = "FULLWRITE";
		this->args = args;
	};

	virtual bool execute() override {
		if (!checkValidArguments())
			return false;

		for (int lba = 0; lba < MAX_SIZE; lba++) {
			callArgs.clear();
			callArgs.push_back("WRITE");
			callArgs.push_back(to_string(lba));
			callArgs.push_back(args[1]);

			ICommand* com = new Write(callArgs);
			com->execute();
		}

		return true;
	}
private:
	virtual bool checkValidArguments() override {
		if (args.size() != 2) return false;

		return _isValidValue(args[1]);
	}


	vector<string> callArgs;
	const int MAX_SIZE = 100;
};
