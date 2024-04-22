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
#include "Read.cpp"
#include "Write.cpp"
#include "Exit.cpp"
#include "Help.cpp"
#include "FullRead.cpp"
#include "FullWrite.cpp"
#include "Logger.hpp"
#include "TestApp.cpp"
#include "Flush.cpp"
#include "Erase.cpp"
#include "EraseRange.cpp"

#define TEST_PASS 0
#define TEST_FAIL 1

using namespace std;

class TestShell {
public:
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

	void selectExecuteMode(int argc, char *argv[]) {
		switch (argc)
		{
		case 1:
			userInputMode();
			break;

		case 2:
			RunnerMode(argv[1]);
			break;

		default:
			break;
		}
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

	void executeCommand(const string userInput) {
		args = parsingInput(userInput);
		string command = args[0];
		
		ICommand* icom{};
		if (command == "WRITE") icom = new Write(args);
		else if (command == "READ") icom = new Read(args);
		else if (command == "EXIT") icom = new Exit(args);
		else if (command == "HELP") icom = new Help(args);
		else if (command == "FULLREAD")	icom = new FullRead(args);
		else if (command == "FULLWRITE") icom = new FullWrite(args);
		else if (command == "FLUSH") icom = new Flush(args);
		else if (command == "ERASE") icom = new Erase(args);
		else if (command == "ERASE_RANGE") icom = new EraseRange(args);
		else icom = new TestApp(args);

		isValid = icom->execute();
		if (isValid == false)
			logger.print("INVALID COMMAND");
	}

	vector<string> getFlieData(string fileName) {
		string filename(fileName);
		vector<string> fileData;
		string line;
		ifstream input_file(filename);

		if (!input_file.is_open()) return fileData;

		while (getline(input_file, line)) {
			fileData.push_back(line);
		}

		input_file.close();
		return fileData;
	}

	void RunnerMode(const string filename) {
		vector<string> TestFileList = getFlieData(filename);

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

protected:
	Logger& logger = Logger::getInstance();
	bool isValid = false;
	vector<string> args;
};
