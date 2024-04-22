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

		_doWrite(stoi(args[1]), args[2]);

		return true;
	}

private:
	void _doWrite(int lba, string data) {
		string command = APP_NAME + " " + "W" + " " + to_string(lba) + " " + data;
		system(command.c_str());
	}

	virtual bool checkValidArguments(vector<string> args) override {
		if (args.size() != 3) return false;
		return _isValidLba(args[1]) && _isValidValue(args[2]);
	}
};