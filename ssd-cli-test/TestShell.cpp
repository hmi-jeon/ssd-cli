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

#define TEST_PASS 0
#define TEST_FAIL 1

using namespace std;

class TestShell {
public:
	TestShell(vector<ICommand*>& cmds) : commands_(cmds) {}

	void prompt(int argc, char* argv[])
	{
		switch (argc)
		{
		case 1:
			userInputMode();
			break;
		case 2:
			runnerMode(argv[1]);
			break;
		default:
			break;
		}
	}

protected:
	Logger& logger = Logger::getInstance();
	vector<string> args;
	vector<ICommand*> commands_;

	vector<string> parsingInput(const string inputString) {
		stringstream ss(inputString);
		vector<string> argList;
		string arg;
		logger.print("Parsing Command string :" + inputString);

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

		if (command == "HELP") {
			printHelp();
			return;
		}

		ICommand* icom = commands_[0];
		for (auto& cmd : commands_) {
			if (args[0] == cmd->getCommandName()) {
				icom = cmd;
				break;
			}
		}

		if (icom->execute(args) == false)
			logger.print("INVALID COMMAND");
	}

	void printHelp() {
		for (auto& cmd : commands_) {
			if (cmd->getCommandName() == "TESTAPP")
				continue;

			cmd->printHelp();
		}
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

	void userInputMode() {
		const int BUF_SIZE = 100;
		char userInput[BUF_SIZE];

		while (1) {
			cout << "> ";
			cin.getline(userInput, BUF_SIZE);
			if (userInput[0] == '\0') continue;
			executeCommand(userInput);
		}
	}

	void runnerMode(const string filename) {
		vector<string> TestFileList = getFlieData(filename);

		logger.setLoggerMode(RUNNER_MODE);
		logger.print("[Runner Mode Start]");
		executeTestScenario(TestFileList);
		logger.print("[Runner Mode End]");
	}

	void executeTestScenario(vector<string> TestFileList) {
		int TestResult = TEST_FAIL;
		string command;

		for (const string TestScenario : TestFileList) {
			cout << TestScenario + "\t---\tRun...";
			command = TestScenario + " " + "RUN";
			TestResult = system(command.c_str());
			if (TestResult != TEST_PASS) {
				cout << "Fail!" << endl;
				break;
			}
			cout << "Pass" << endl;
		}
	}

	
};
