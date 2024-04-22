#pragma once
#include "ICommand.cpp"
#include <iostream>
#include <fstream>
#include <vector>

class Read : public ICommand {
public:
	Read() {
		this->name = "READ";
	};

	virtual bool execute(vector<string> args) override {
		if (!checkValidArguments(args))
			return false;

		// ssd.exe call
		string command = APP_NAME + " " + "R" + " " + args[1];
		system(command.c_str());

		// result.txt open
		ifstream resultFile;
		string data = "";
		resultFile.open("result.txt");
		if (resultFile.is_open())
			resultFile >> data;
		cout << data << endl;

		return true;
	};

private:
	virtual bool checkValidArguments(vector<string> args) override {
		if (args.size() != 2) return false;
		return _isValidLba(args[1]);
	}
};