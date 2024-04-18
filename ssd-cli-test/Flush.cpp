#pragma once
#include "ICommand.cpp"
#include <vector>

class Flush : public ICommand {
public:
	Flush(vector<string> args) {
		this->name = "FLUSH";
		this->args = args;
	};

	virtual bool execute() override {
		if (!checkValidArguments())
			return false;

		string command = APP_NAME + " " + "F";
		system(command.c_str());

		return true;
	}

private:
	virtual bool checkValidArguments() override {
		return (args.size() == 1);
	}
};