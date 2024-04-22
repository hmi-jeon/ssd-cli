#pragma once
#include "ICommand.cpp"

class Flush : public ICommand {
public:
	Flush() {
		this->name = "FLUSH";
		this->description = "The data in the write buffer are applied to the SSD.";
	};

	virtual bool execute(vector<string> args) override {
		if (!checkValidArguments(args))
			return false;

		string command = APP_NAME + " " + "F";
		system(command.c_str());

		return true;
	}

private:
	virtual bool checkValidArguments(vector<string> args) override {
		return (args.size() == 1);
	}
};