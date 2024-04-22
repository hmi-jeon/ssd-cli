#pragma once
#include "ICommand.cpp"
#include <vector>

class Write : public ICommand {
public:
	Write() {
		this->name = "WRITE";
	};

	virtual bool execute(vector<string> args) override {
		if (!checkValidArguments(args))
			return false;

		string command = APP_NAME + " " + "W" + " " + args[1] + " " + args[2];
		system(command.c_str());

		return true;
	}

private:
	virtual bool checkValidArguments(vector<string> args) override {
		if (args.size() != 3) return false;
		return _isValidLba(args[1]) && _isValidValue(args[2]);
	}
};