#pragma once
#include "ICommand.cpp"
#include <vector>
#include "Write.cpp"

class FullWrite : public ICommand {
public:
	FullWrite() {
		this->name = "FULLWRITE";
	};

	virtual bool execute(vector<string> args) override {
		if (!checkValidArguments(args))
			return false;

		for (int lba = 0; lba < MAX_LBA; lba++) {
			callArgs.clear();
			callArgs.push_back("WRITE");
			callArgs.push_back(to_string(lba));
			callArgs.push_back(args[1]);

			ICommand* com = new Write();
			com->execute(args);
		}

		return true;
	}
private:
	virtual bool checkValidArguments(vector<string> args) override {
		if (args.size() != 2) return false;

		return _isValidValue(args[1]);
	}


	vector<string> callArgs;
};
