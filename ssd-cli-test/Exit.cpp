#pragma once
#include "ICommand.cpp"
#include <vector>

class Exit : public ICommand {
public:
	Exit(vector<string> args) {
		this->name = "EXIT";
		this->args = args;
	};

	void execute() override {
		exit(0);
	}
};