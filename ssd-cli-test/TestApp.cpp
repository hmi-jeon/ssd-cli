#pragma once
#pragma warning(disable: 4996)
#define _CRT_SECURE_NO_WARNINGS 1
#include "ICommand.cpp"
#include <vector>
#include <iostream>
#include <io.h>

class TestApp : public ICommand {
public:
	TestApp(vector<string> args) {
		this->name = "TEST_SCENARIO_APPLICATIONS";
		this->args = args;
	};

	virtual bool execute() override {
		if (!checkValidArguments())
			return false;

		string appName = args[0];
		appName = appName + ".exe";
		char fileName[100];
		strcpy(fileName, appName.c_str());

		if (access(fileName, 0) == -1)
			return false;
		if (system(fileName))
			return false;

		return true;
	}
private:
	virtual bool checkValidArguments() override {
		return (args.size() == 1);
	}
};