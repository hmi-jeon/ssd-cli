#pragma once
#include "ICommand.cpp"
#include <vector>

class Write : public ICommand {
public:
	Write(vector<string> args) {
		this->name = "WRITE";
		this->args = args;
	};

	virtual bool execute() override {
		if (!checkValidArguments())
			return false;

		string fileName = "ssd-cli.exe";
		string command = fileName + " " + "W" + " " + args[1] + " " + args[2];
		system(command.c_str());

		return true;
	}

private:
	virtual bool checkValidArguments() override {
		if (args.size() != 3) return false;
		return _isValidLba(args[1]) && _isValidValue(args[2]);
	}
};