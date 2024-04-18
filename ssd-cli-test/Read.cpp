#pragma once
#include "ICommand.cpp"
#include <iostream>
#include <fstream>
#include <vector>

class Read : public ICommand {
public:
	Read(vector<string> args) {
		this->name = "READ";
		this->args = args;
	};

	void execute() override {
		// ssd.exe call
		string fileName = "ssd-cli.exe";
		string command = fileName + " " + "R" + " " + args[1];
		system(command.c_str());

		// result.txt open
		ifstream resultFile;
		string data = "";
		resultFile.open("result.txt");
		if (resultFile.is_open())
			resultFile >> data;
		cout << data << endl;
	};
};