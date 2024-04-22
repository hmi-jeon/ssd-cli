#pragma once
#include "ICommand.cpp"
#include <vector>

class Exit : public ICommand {
public:
	Exit() {
		this->name = "EXIT";
		this->description = "Exit SHELL. (ex. EXIT)";
	};

	virtual bool execute(vector<string> args) override {
		if (!checkValidArguments(args))
			return false;
		exit(0);
	}

private:
	virtual bool checkValidArguments(vector<string> args) override {
		return (args.size() == 1);
	}
};