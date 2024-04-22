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

		_doRead(stoi(args[1]));

		_printResult();

		return true;
	};

private:
	void _doRead(int lba) {
		string command = APP_NAME + " " + "R" + " " + to_string(lba);
		system(command.c_str());
	}

	void _printResult() {
		ifstream resultFile;
		string data = "";
		resultFile.open("result.txt");
		if (resultFile.is_open())
			resultFile >> data;
		cout << data << endl;
	}

	virtual bool checkValidArguments(vector<string> args) override {
		if (args.size() != 2) return false;
		return _isValidLba(args[1]);
	}
};