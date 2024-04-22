#pragma once
#pragma warning(disable: 4996)
#define _CRT_SECURE_NO_WARNINGS 1
#include "ICommand.cpp"
#include <vector>
#include <iostream>
#include <io.h>

class TestApp : public ICommand {
public:
	TestApp() {
		this->name = "TEST_SCENARIO_APPLICATIONS";
	};

	virtual bool execute(vector<string> args) override {
		if (!checkValidArguments(args))
			return false;

		string appName = args[0];
		appName = appName + ".exe";

		logger.print("excute TestApp Scenario:"+ appName);

		char fileName[100];
		strcpy(fileName, appName.c_str());

		if (access(fileName, 0) == -1) {
			logger.print("File Access Fail!");
			return false;
		}
		if (system(fileName)) {
			logger.print("Test APP Fail!!");
			return false;
		}

		return true;
	}
private:
	virtual bool checkValidArguments(vector<string> args) override {
		return (args.size() == 1);
	}
};