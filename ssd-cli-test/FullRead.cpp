#pragma once
#include "ICommand.cpp"
#include <vector>
#include "Read.cpp"


class FullRead : public ICommand {
public:
	FullRead(vector<string> args) {
		this->name = "FULLWRITE";
		this->args = args;
	};

	void execute() override {
		for (int lba = 0; lba < 100; lba++) {
			callArgs.clear();
			callArgs.push_back("READ");
			callArgs.push_back(to_string(lba));

			ICommand* com = new Read(callArgs);
			com->execute();
		}
	}
private:
	vector<string> callArgs;
	const int MAX_SIZE = 100;
};
