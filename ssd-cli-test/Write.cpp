#pragma once
#include "ICommand.cpp"
#include <vector>

class Write : public ICommand {
public:
	Write(vector<string> args) {
		this->name = "WRITE";
		this->args = args;
	};

	void execute() override {
		string fileName = "ssd-cli.exe";
		string command = fileName + " " + "W" + " " + args[1] + " " + args[2];
		system(command.c_str());
	}
};