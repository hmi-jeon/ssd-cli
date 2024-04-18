#pragma once
#include "ICommand.cpp"
#include <vector>

class Exit : public ICommand {
public:
	Exit(vector<string> args) {
		this->name = "EXIT";
		this->args = args;
	};

	virtual bool execute() override {
		if (!checkValidArguments())
			return false;
		exit(0);
	}

private:
	virtual bool checkValidArguments() override {
		return (args.size() == 1);
	}
};