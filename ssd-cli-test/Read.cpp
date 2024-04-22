#pragma once
#include "ICommand.cpp"
#include <iostream>
#include <fstream>
#include <vector>

class Read : public ICommand {
public:
	Read() {
		this->name = "READ";
		this->description = "Outputs data written to the LBA address value of the device. (ex. READ 3)";
	};

	virtual bool execute(vector<string> args) override {
		if (!checkValidArguments(args))
			return false;

		_doRead(stoi(args[1]));

		_printResult();

		return true;
	};

protected:
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

private:
	virtual bool checkValidArguments(vector<string> args) override {
		if (args.size() != 2) return false;
		return _isValidLba(args[1]);
	}
};