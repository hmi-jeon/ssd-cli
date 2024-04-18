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
#include "TestApp.cpp"
#include "Flush.cpp"
#include "Erase.cpp"
#include "Logger.cpp"

#define RUN_LIST "run_list.lst"
#define TEST_PASS 0
#define TEST_FAIL 1

using namespace std;

interface ISSD {
	virtual string read(const int lba) = 0;
	virtual void write(const int lba, const string data) = 0;
};

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

	void executeCommand() {
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
		else icom = new TestApp(args);

		isValid = icom->execute();
		if (isValid == false)
			_printInvalidCommand();
	}

	void inputCommand(const string userInput) {
		args = parsingInput(userInput);
		executeCommand();
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

protected:
	void _printInvalidCommand() {
		std::cout << "INVALID COMMAND" << std::endl;
	}

	Logger& logger = Logger::getInstance();
	bool isValid = false;
	vector<string> args;
};
