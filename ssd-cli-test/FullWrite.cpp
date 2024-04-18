#pragma once
#include "ICommand.cpp"
#include <vector>
#include "Write.cpp"

class FullWrite : public ICommand {
public:
	FullWrite(vector<string> args) {
		this->name = "FULLWRITE";
		this->args = args;
	};

	void execute() override {
		for (int lba = 0; lba < MAX_SIZE; lba++) {
			callArgs.push_back("WRITE");
			callArgs.push_back(to_string(lba));
			callArgs.push_back(args[1]);

			ICommand* com = new Write(callArgs);
			com->execute();
		}
	}
private:
	vector<string> callArgs;
	const int MAX_SIZE = 100;
};