#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <process.h>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include "ICommand.cpp"
#include "Logger.hpp"

#define RUN_LIST "run_list.lst"
#define TEST_PASS 0
#define TEST_FAIL 1

using namespace std;

class TestShell {
public:
	TestShell(vector<ICommand*>& cmds) : commands_(cmds) {}

	void prompt(int argc, char* argv[])
	{
		if (argc == 2) {
			string firstArg = argv[1];
			if (firstArg == RUN_LIST) Runner();
		}

		if (argc == 1) {
			char userInput[100];

			while (1) {
				cout << "> ";
				cin.getline(userInput, 100);
				if (userInput[0] == '\0') {
					continue;
				}
				executeCommand(userInput);
			}
		}
	}

protected:
	Logger& logger = Logger::getInstance();
	bool isValid = false;
	vector<string> args;
	vector<ICommand*> commands_;

	vector<string> parsingInput(const string inputString) {
		stringstream ss(inputString);
		vector<string> argList;
		string arg;

		while (getline(ss, arg, ' ')) {
			argList.push_back(arg);
		}

		if (argList.size() > 0) {
			transform(argList[0].begin(), argList[0].end(), argList[0].begin(), ::toupper);
		}

		return argList;
	}

	void executeCommand(const string userInput) {
		args = parsingInput(userInput);
		string command = args[0];
		ICommand* icom = commands_[0];
		for (auto& cmd : commands_) {
			if (args[0] == cmd->getCommandName()) {
				icom = cmd;
			}
		}

		isValid = icom->execute(args);
		if (isValid == false)
			logger.print("INVALID COMMAND");
	}

	vector<string> getFlieData(string fileName) {
		string filename(fileName);
		vector<string> lines;
		string line;

		ifstream input_file(filename);

		if (!input_file.is_open()) return lines;

		while (getline(input_file, line)) {
			lines.push_back(line);
		}

		input_file.close();

		return lines;
	}

	void Runner() {
		vector<string> TestFileList = getFlieData(RUN_LIST);

		logger.setLoggerMode(RUNNER_MODE);

		int TestResult = TEST_FAIL;

		for (const string TestScenario : TestFileList) {
			cout << TestScenario + "\t---\tRun...";
			TestResult = system(TestScenario.c_str());
			if (TestResult != TEST_PASS) {
				cout << "Fail!" << endl;
				break;
			}

			cout << "Pass" << endl;
		}
	}
};
